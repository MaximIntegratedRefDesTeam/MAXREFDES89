/******************************************************************//**
* @file maxrefdes89.h
*
* @author Justin Jordan
*
* @version 0.0
*
* Started: 18JUL15
*
* Updated: 
* 
* 22JAN16 - ported to Arduino
*
* @brief Source file for maxrefdes89 
***********************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************/


#include "maxrefdes89.h"


//Motor Driver control inputs
#define MD_EN    (0x01)
#define MD_DIR   (0x02)
#define MD_MODE0 (0x04)
#define MD_MODE1 (0x08)

#define MAX_VREF (0x9B)

//GPIO Expander Default Configurations
#define MAX7300_ALL_OUTPUTS    (0x55)
#define MAX7300_ALL_INPUTS     (0xFF)
#define MAX7300_OUTPUT_DEFAULT (0xBB)


//*********************************************************************
Maxrefdes89::Maxrefdes89()
{
  
}


//*********************************************************************
void Maxrefdes89::begin(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3, uint8_t pwm4)
{
  _md1_pwm = pwm1;
  _md2_pwm = pwm2;
  _md3_pwm = pwm3;
  _md4_pwm = pwm4;

  _p_io_expander = new Max7300;
  _p_io_expander->begin(Max7300::MAX7300_I2C_ADRS0);
  _p_digi_pot1 = new Max5387;
  _p_digi_pot1->begin(Max5387::MAX5387_I2C_ADRS0);
  _p_digi_pot2 = new Max5387;
  _p_digi_pot1->begin(Max5387::MAX5387_I2C_ADRS1);
  
  
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_04, MAX7300_ALL_OUTPUTS);
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_08, MAX7300_ALL_OUTPUTS);
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_12, MAX7300_ALL_OUTPUTS);
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_16, MAX7300_ALL_OUTPUTS);
  
  //Set /EN and DIR pin of all motor drivers and set mode pin 
  //of all motor drivers to 0.75V
  _p_io_expander->write_8_ports(Max7300::MAX7300_PORT_04, MAX7300_OUTPUT_DEFAULT);
  _p_io_expander->write_8_ports(Max7300::MAX7300_PORT_12, MAX7300_OUTPUT_DEFAULT);
    
  //configure these ports as inputs w/pull-up, 
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_20, MAX7300_ALL_INPUTS);
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_24, MAX7300_ALL_INPUTS);
  _p_io_expander->config_4_ports(Max7300::MAX7300_PORT_28, MAX7300_ALL_INPUTS);
  
  //config port 31 as output for interrupt
  _p_io_expander->config_port(Max7300::MAX7300_PORT_31, Max7300::MAX7300_PORT_OUTPUT);
    
  _p_io_expander->enable_transition_detection();
  _p_io_expander->enable_ports();
    
  //set Vref pin of all motor drivers to 0V, 
  //internal vref used for current regulation
  _p_digi_pot1->write_ch_AB(0);
  _p_digi_pot2->write_ch_AB(0);

  pinMode(_md1_pwm, OUTPUT);
  pinMode(_md2_pwm, OUTPUT);
  pinMode(_md3_pwm, OUTPUT);
  pinMode(_md4_pwm, OUTPUT);
    
  //init motor data to defaults
  for(uint8_t idx = 0; idx < 4; idx++)
  {
    _motor_data_array[idx].op_mode = COAST;
    _motor_data_array[idx].i_reg_mode = RIPPLE_25_INTERNAL_REF;
    _motor_data_array[idx].duty_cycle = 0;
    _motor_data_array[idx].v_ref = 0;
  }
}


//*********************************************************************
void Maxrefdes89::end()
{
  delete _p_io_expander;
  delete _p_digi_pot1;
  delete _p_digi_pot1;
}


//*********************************************************************
uint8_t Maxrefdes89::set_operating_mode(max14871_motor_driver_t md, 
                                        max14871_operating_mode_t mode)
{
    uint8_t result = 0;
    uint8_t port_data;
    
    Max7300::max7300_port_number_t low_port;
    
    //determine the low port of an 8 bit register to read/write 
    if(md < MD3)
    {
        low_port = Max7300::MAX7300_PORT_04;
    }
    else
    {
        low_port = Max7300::MAX7300_PORT_12;
    }
    
    //get current state of outputs
    port_data = _p_io_expander->read_8_ports(low_port);

    switch(mode)
    {
        //if(md % 2) for following cases, modify control bits
        //of odd motor driver

        case COAST:
            if(md % 2)
            {
                port_data |= MD_EN;
            }
            else
            {
                port_data |= (MD_EN << 4);
            }
            
            set_pwm_duty_cycle(md, 0.0f);
        break;
        
        case BRAKE:
            if(md % 2)
            {
                port_data &= ~MD_EN;
            }
            else
            {
                port_data &= ~(MD_EN << 4);
            }
            
            set_pwm_duty_cycle(md, 0.0f);
        break;
        
        case REVERSE:
            if(md % 2)
            {
                port_data &= ~(MD_EN + MD_DIR);
            }
            else
            {
                port_data &= ~((MD_EN + MD_DIR) << 4);
            }
        break;
        
        case FORWARD:
            if(md % 2)
            {
                port_data &= ~MD_EN;
                port_data |= MD_DIR;
            }
            else
            {
                port_data &= ~(MD_EN << 4);
                port_data |= (MD_DIR << 4);
            } 
        break;
        
        default:
            result = -1;
        break;
    }
    
    if(!result)
    {
        //write data back to port
        result = _p_io_expander->write_8_ports(low_port, (uint8_t) port_data);
        
        if(!result)
        {
            _motor_data_array[(md - 1)].op_mode = mode;
        }
    }

    return result;
}


//*********************************************************************
uint8_t Maxrefdes89::set_current_regulation_mode(max14871_motor_driver_t md, 
                                    max14871_current_regulation_mode_t mode,
                                    uint8_t vref)
{
    uint8_t result = 0;
    uint8_t port_data;
    uint8_t local_vref = 0;
    
    Max7300::max7300_port_number_t low_port;
    Max5387 *p_digi_pot;
    
    if(vref > MAX_VREF)
    {
        vref = MAX_VREF;
    }
    local_vref = ((uint8_t) ((vref * 255.0f) / 3.3f));
    
    //determine the low port of an 8 bit register to read/write 
    //and digipot associated with motor driver
    if(md < MD3)
    {
        low_port = Max7300::MAX7300_PORT_04;
        p_digi_pot = _p_digi_pot1;
    }
    else
    {
        low_port = Max7300::MAX7300_PORT_12;
        p_digi_pot = _p_digi_pot2;
    }
    
    //get current state of outputs
    port_data = _p_io_expander->read_8_ports(low_port);
        
    switch(mode)
    {
        case RIPPLE_25_INTERNAL_REF:
            if(md % 2)
            {
                port_data &= ~MD_MODE0;
                port_data |= MD_MODE1;
                p_digi_pot->write_ch_A(0);
            }
            else
            {
                port_data &= ~(MD_MODE0 << 4);
                port_data |= (MD_MODE1 << 4);
                p_digi_pot->write_ch_B(0);
            }
        break;
        
        case RIPPLE_25_EXTERNAL_REF:
            if(md % 2)
            {
                port_data &= ~MD_MODE0;
                port_data |= MD_MODE1;
                p_digi_pot->write_ch_A(local_vref);
            }
            else
            {
                port_data &= ~(MD_MODE0 << 4);
                port_data |= (MD_MODE1 << 4);
                p_digi_pot->write_ch_B(local_vref);
            }
        break;
        
        case TCOFF_FAST_INTERNAL_REF:
            if(md % 2)
            {
                port_data |= (MD_MODE1 + MD_MODE0);
                p_digi_pot->write_ch_A(0);
            }
            else
            {
                port_data |= ((MD_MODE1 + MD_MODE0) << 4);
                p_digi_pot->write_ch_B(0);
            }
        break;
        
        case TCOFF_SLOW_INTERNAL_REF:
            if(md % 2)
            {
                port_data |= MD_MODE0;
                port_data &= ~MD_MODE1;
                p_digi_pot->write_ch_A(0);
            }
            else
            {
                port_data |= (MD_MODE0 << 4);
                port_data &= ~(MD_MODE1 << 4);
                p_digi_pot->write_ch_B(0);
            }
        break;
        
        case TCOFF_FAST_EXTERNAL_REF:
            if(md % 2)
            {
                port_data |= (MD_MODE1 + MD_MODE0);
                p_digi_pot->write_ch_A(local_vref);
            }
            else
            {
                port_data |= ((MD_MODE1 + MD_MODE0) << 4);
                p_digi_pot->write_ch_B(local_vref);
            }
        break;
        
        case TCOFF_SLOW_EXTERNAL_REF:
            if(md % 2)
            {
                port_data |= MD_MODE0;
                port_data &= ~MD_MODE1;
                p_digi_pot->write_ch_A(local_vref);
            }
            else
            {
                port_data |= (MD_MODE0 << 4);
                port_data &= ~(MD_MODE1 << 4);
                p_digi_pot->write_ch_B(local_vref);
            }
        break;
        
        default:
            result = -1;
        break;
    }
    
    if(!result)
    {
        //write data back to port
        result = _p_io_expander->write_8_ports(low_port, (uint8_t) port_data);
        
        if(!result)
        {
            _motor_data_array[(md - 1)].i_reg_mode = mode;
            _motor_data_array[(md - 1)].v_ref = vref;
        }
    }
    
    return result;
}


//*********************************************************************
uint8_t Maxrefdes89::set_pwm_duty_cycle(max14871_motor_driver_t md, uint8_t duty_cycle)
{
    uint8_t result = 0;
    
    switch(md)
    {
        case MD1:
            analogWrite(_md1_pwm, duty_cycle);
            _motor_data_array[(md - 1)].duty_cycle = duty_cycle;
        break;
        
        case MD2:
            analogWrite(_md2_pwm, duty_cycle);
            _motor_data_array[(md - 1)].duty_cycle = duty_cycle;
        break;
        
        case MD3:
            analogWrite(_md3_pwm, duty_cycle);
            _motor_data_array[(md - 1)].duty_cycle = duty_cycle;
        break;
        
        case MD4:
            analogWrite(_md4_pwm, duty_cycle);
            _motor_data_array[(md - 1)].duty_cycle = duty_cycle;
        break;
        
        default:
            result = 0xFF;
        break;
    }
    
    return result;
}


//*********************************************************************
Maxrefdes89::max14871_operating_mode_t Maxrefdes89::get_operating_mode(max14871_motor_driver_t md)
{
    return(_motor_data_array[(md - 1)].op_mode);
}


//*********************************************************************
Maxrefdes89::max14871_current_regulation_mode_t Maxrefdes89::get_current_regulation_mode(max14871_motor_driver_t md)
{
    return(_motor_data_array[(md - 1)].i_reg_mode);
}


//*********************************************************************
uint8_t Maxrefdes89::get_pwm_duty_cycle(max14871_motor_driver_t md)
{
    return(_motor_data_array[(md - 1)].duty_cycle);
}


//*********************************************************************
uint8_t Maxrefdes89::get_external_voltage_ref(max14871_motor_driver_t md)
{
    return(_motor_data_array[(md - 1)].v_ref);
}

