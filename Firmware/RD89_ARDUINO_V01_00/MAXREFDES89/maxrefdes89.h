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
* @brief Header file for maxrefdes89 
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


#ifndef MAXREFDES89_H
#define MAXREFDES89_H


#include <Arduino.h>
#include <max5387.h>
#include <max7300.h>


class Maxrefdes89
{
  public:

    typedef enum
    {
        MD1 = 1,
        MD2,
        MD3,
        MD4
    }max14871_motor_driver_t;
    
    
    typedef enum
    {
        COAST = 1,
        BRAKE,
        REVERSE,
        FORWARD
    }max14871_operating_mode_t;
    
    
    typedef enum
    {
        RIPPLE_25_INTERNAL_REF = 1,
        RIPPLE_25_EXTERNAL_REF,
        TCOFF_FAST_INTERNAL_REF,
        TCOFF_SLOW_INTERNAL_REF,
        TCOFF_FAST_EXTERNAL_REF,
        TCOFF_SLOW_EXTERNAL_REF
    }max14871_current_regulation_mode_t;


    /**********************************************************//**
    * @brief Maxrefdes89 constructor
    *
    * @details 
    *
    * On Entry:
    *    
    * On Exit:
    *    @return 
    **************************************************************/
    Maxrefdes89();


    /**********************************************************//**
    * @brief Use in setup, initializes board
    *
    * @details 
    *
    * On Entry:
    *     @param[in] pwm1 - GPIO used for MD1 pwm signal
    *     @param[in] pwm2 - GPIO used for MD2 pwm signal
    *     @param[in] pwm3 - GPIO used for MD3 pwm signal
    *     @param[in] pwm4 - GPIO used for MD4 pwm signal
    *    
    * On Exit:
    *    @return 
    **************************************************************/
    void begin(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3, uint8_t pwm4);


    /**********************************************************//**
    * @brief deletes private objects
    *
    * @details 
    *
    * On Entry:
    *    
    * On Exit:
    *    @return 
    **************************************************************/
    void end();


    /**********************************************************//**
    * @brief set mode of operation for given motor driver, md
    *
    * @details 
    *
    * On Entry:
    *     @param[in] md - motor driver
    *     @param[in] mode - mode of operation
    *    
    * On Exit:
    *    @return 0 on success, non-0 on failure
    **************************************************************/
    uint8_t set_operating_mode(max14871_motor_driver_t md, 
                               max14871_operating_mode_t mode);


    /**********************************************************//**
    * @brief sets current regulation mode
    *
    * @details 
    *
    * On Entry:
    *     @param[in] md - motor driver
    *     @param[in] mode - current regulation mode
    *    
    * On Exit:
    *    @return 0 on success, non-0 on failure
    **************************************************************/
    uint8_t set_current_regulation_mode(max14871_motor_driver_t md, 
                                        max14871_current_regulation_mode_t mode,
                                        uint8_t vref);


    /**********************************************************//**
    * @brief set pwm duty cycle of given motor driver
    *
    * @details 
    *
    * On Entry:
    *     @param[in] md - motor driver
    *     @param[in] duty_cycle - 0 to 255 = 0 to 100%
    *    
    * On Exit:
    *    @return 0 on success, non-0 on failure
    **************************************************************/
    uint8_t set_pwm_duty_cycle(max14871_motor_driver_t md, uint8_t duty_cycle);


    /**********************************************************//**
    * @brief Get operating mode of selected motor driver
    *
    * @details 
    *
    * On Entry:
    *    @param[in] md - 1 of 4 motor drivers on the shield
    *    
    * On Exit:
    *    @return operating mode of selected motor driver
    **************************************************************/
    max14871_operating_mode_t get_operating_mode(max14871_motor_driver_t md);
    
    
    /**********************************************************//**
    * @brief Get current regulation mode of selected motor driver
    *
    * @details 
    *
    * On Entry:
    *    @param[in] md - 1 of 4 motor drivers on the shield
    *    
    * On Exit:
    *    @return current regulation mode of selected motor driver
    **************************************************************/
    max14871_current_regulation_mode_t get_current_regulation_mode(max14871_motor_driver_t md);
    
    
    /**********************************************************//**
    * @brief Get duty cycle of selected motor driver
    *
    * @details 
    *
    * On Entry:
    *    @param[in] md - 1 of 4 motor drivers on the shield
    *    
    * On Exit:
    *    @return duty cycle of selected motor driver
    **************************************************************/
    uint8_t get_pwm_duty_cycle(max14871_motor_driver_t md);
    
    
    /**********************************************************//**
    * @brief Get external voltage reference of selected motor driver
    *
    * @details 
    *
    * On Entry:
    *    @param[in] md - 1 of 4 motor drivers on the shield
    *    
    * On Exit:
    *    @return external voltage reference of selected motor driver
    **************************************************************/
    uint8_t get_external_voltage_ref(max14871_motor_driver_t md);


  private:

    struct motor_data_s
    {
        max14871_operating_mode_t op_mode;
        max14871_current_regulation_mode_t i_reg_mode;
        uint8_t duty_cycle;
        uint8_t v_ref;
    };
    
    struct motor_data_s _motor_data_array[4];

    Max7300 *_p_io_expander;
    Max5387 *_p_digi_pot1;
    Max5387 *_p_digi_pot2;

    //default pwm channels
    uint8_t _md1_pwm;
    uint8_t _md2_pwm; 
    uint8_t _md3_pwm;
    uint8_t _md4_pwm;
};

#endif /*MAXREFDES89_H*/

