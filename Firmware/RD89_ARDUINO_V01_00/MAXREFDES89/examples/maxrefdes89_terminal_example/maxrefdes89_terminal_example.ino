/***********************************************************************
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


#include <Arduino.h>
#include <maxrefdes89.h>

void menu_print_motor_data(uint8_t md);
uint8_t get_user_int(char *msg, uint8_t max_val);
uint8_t menu_select_motor_driver(void);
void menu_set_duty_cycle_(uint8_t md);
void menu_set_op_mode(uint8_t md);
void menu_set_current_reg(uint8_t md);
void menu_print_motor_data(uint8_t md);
void clear_screen(void);


//do not use () for constructor that does not require arguments
//could be interpeted by the compiler as a function declaration
//with the class as the return type, your intended object as the 
//function name, and no arguments, i.e. void

// !!!! Dont do this -> Maxrefdes89 motor_shield(); !!!! 

Maxrefdes89 motor_shield; 

//These constants represent the digital outputs used for pwm
//The default GPIO used are D4, D5, D9, D10 from the factory
//the shield supports D3, D4, D5, D6, D8, D9, D10, D11
//through hardware configuration, 0 ohm resistors, please see the schematic

//setup for Arduino Uno, 
//the 0 ohm resistor connected to D4 on MAXREFDES89# must be moved to D3 position 
const uint8_t MD1_PWM = 3;
const uint8_t MD2_PWM = 5;
const uint8_t MD3_PWM = 9;
const uint8_t MD4_PWM = 10;

uint8_t idx = 0;


void setup() 
{
  Serial.begin(115200);

  //init object
  motor_shield.begin(MD1_PWM, MD2_PWM, MD3_PWM, MD4_PWM);
  
  uint8_t idx = 0;
  uint8_t user_entry = 0;
    
  idx = 1;
  
  //main menu                    
  while(user_entry != 5)
  {
      Serial.println();
      Serial.println(F("MAXREFDES89# Demo Program"));
      Serial.println();

      //print currently selected motor's data
      menu_print_motor_data(idx);

      Serial.println();
      Serial.println(F("Please select an option below:"));
      Serial.println(F("1. Select Active Motor Driver"));
      Serial.println(F("2. Set PWM Duty Cycle"));
      Serial.println(F("3. Set Operating Mode"));
      Serial.println(F("4. Set Current Regulation Mode"));
      Serial.println(F("5. Quit\n"));

      user_entry = get_user_int(NULL, 5);
      
      switch(user_entry)
      {
          case 1:
              idx = menu_select_motor_driver();
              clear_screen();
          break;
          
          case 2:
              menu_set_duty_cycle_(idx);
              clear_screen();
          break;
          
          case 3:
              menu_set_op_mode(idx);
              clear_screen();
          break;
          
          case 4:
              menu_set_current_reg(idx);
              clear_screen();
          break;
          
          case 5:
              Serial.println(F("Ending Program"));
          break;
          
          default:
              Serial.println(F("Invalid entry, please try again"));
          break;
      }
  }

  //enter endless loop()
}


void loop() 
{
}


//*********************************************************************
void menu_print_motor_data(uint8_t md)
{
    Maxrefdes89::max14871_operating_mode_t op_state;
    Maxrefdes89::max14871_current_regulation_mode_t reg_state;
    uint16_t dc;
    uint16_t v_ref;
    
    op_state = motor_shield.get_operating_mode((Maxrefdes89::max14871_motor_driver_t) md);
        
    reg_state = motor_shield.get_current_regulation_mode((Maxrefdes89::max14871_motor_driver_t) md);
        
    dc = motor_shield.get_pwm_duty_cycle((Maxrefdes89::max14871_motor_driver_t) md);
        
    v_ref = motor_shield.get_external_voltage_ref((Maxrefdes89::max14871_motor_driver_t) md);
    
    Serial.print(F("Active Motor Driver = "));
    Serial.println(md, DEC);
    
    switch(op_state)
    {
        case 1:
            Serial.println(F("Operating Mode = COAST"));
        break;
        
        case 2:
            Serial.println(F("Operating Mode = BRAKE"));
        break;
        
        case 3:
            Serial.println(F("Operating Mode = REVERSE"));
        break;
        
        case 4:
            Serial.println(F("Operating Mode = FORWARD"));
        break;
        
        default:
            Serial.println(F("Operating Mode = UNKNOWN"));
        break;
    }
    
    
    switch(reg_state)
    {
        case 1:
            Serial.println(F("Current Regulation Mode = 25% ripple, internal reference"));
        break;
        
        case 2:
            Serial.println(F("Current Regulation Mode = 25% ripple, external reference"));
        break;
         
        case 3:
            Serial.println(F("Current Regulation Mode = TCOFF fast decay, internall reference"));
        break;
        
        case 4:
            Serial.println(F("Current Regulation Mode = TCOFF slow decay, internall reference"));
        break;
        
        case 5:
            Serial.println(F("Current Regulation Mode = TCOFF fast decay, external reference"));
        break;
        
        case 6:
            Serial.println(F("Current Regulation Mode = TCOFF slow decay, external reference"));
        break;
        
        default:
            Serial.println(F("Current Regulation Mode = UNKNOWN"));
        break;
    }
    
    Serial.print(F("Duty Cycle = "));
    dc = (dc * 100)/255; //good enough for demo
    Serial.print(dc, DEC);
    Serial.println("%");

    Serial.print(F("Voltage Reference = "));
    v_ref = (13 * v_ref);
    Serial.print(v_ref/1000, DEC);
    v_ref = (v_ref - ((v_ref/1000)*1000)); //(x/1000)*1000 actually does something in integer math
    Serial.print(".");
    Serial.print(v_ref/100, DEC);
    Serial.println("V");
}


//*********************************************************************
uint8_t get_user_int(char *msg, uint8_t max_val)
{
  uint8_t user_int;
  uint8_t bytes_available;
  uint8_t input_buffer[64];

  do
  {
    Serial.println(msg);
    do
    {
      bytes_available = Serial.readBytesUntil(0x0A, input_buffer, 64);
    }
    while(!bytes_available);

    if(bytes_available > 4)
    {
      Serial.println(F("Invalid Entry"));
      user_int = 0xFF;
    }
    else if(bytes_available > 3)
    {
      user_int = (100*(input_buffer[0] - '0')); //get 100s place
      user_int += (10*(input_buffer[1] - '0')); //get 10s place
      user_int += (input_buffer[2] - '0'); //get 1s place
    }
    else if(bytes_available > 2)
    {
      user_int = (10*(input_buffer[0] - '0'));
      user_int += (input_buffer[1] - '0');
    }
    else
    {
      user_int = (input_buffer[0] - '0');
    }

    Serial.print(F("You entered "));
    Serial.println(user_int, DEC);
    if(user_int > max_val)
    {
      Serial.println(F("Value out of range"));
    }
    
  }
  while(user_int > max_val);

  return(user_int);
}


//*********************************************************************
uint8_t menu_select_motor_driver(void)
{
  uint8_t md;
  
  do
  {
    md = get_user_int("Please enter motor driver number, 1-4", 4);
  }
  while((md < 1) || (md > 4));
  
  return md;
}


//*********************************************************************
void menu_set_duty_cycle_(uint8_t md)
{
  
  uint8_t dc = 0;
  
  dc = get_user_int("Please enter new duty cycle, 0 - 255 = (0 - 100%)", 255);
  
  motor_shield.set_pwm_duty_cycle((Maxrefdes89::max14871_motor_driver_t) md, dc);
}


//*********************************************************************
void menu_set_op_mode(uint8_t md)
{
    uint8_t mode;
    
    do
    {
        Serial.println(F("Please enter one of the following modes:"));
        Serial.println(F("1.COAST"));
        Serial.println(F("2.BRAKE"));
        Serial.println(F("3.REVERSE"));
        Serial.println(F("4.FORWARD"));
        
        mode = get_user_int(NULL, 4);
    }
    while((mode < 1) || (mode > 4));
    
    motor_shield.set_operating_mode((Maxrefdes89::max14871_motor_driver_t) md,(Maxrefdes89::max14871_operating_mode_t) mode);
}


//*********************************************************************
void menu_set_current_reg(uint8_t md)
{
    uint8_t mode;
    uint8_t v_ref = 0;
    
    do
    {
        Serial.println(F("Please enter one of the following modes:"));
        Serial.println(F("1. 25%% ripple, internal reference"));
        Serial.println(F("2. 25%% ripple, external reference"));
        Serial.println(F("3. TCOFF fast decay, internall reference"));
        Serial.println(F("4. TCOFF slow decay, internall reference"));
        Serial.println(F("5. TCOFF fast decay, external reference"));
        Serial.println(F("6. TCOFF slow decay, external reference"));
        
        mode = get_user_int(NULL, 6);
    }
    while((mode < 1) || (mode > 6));
    
    if((mode == 2) || (mode >= 5))
    {
        do
        {
            Serial.println(F("Please enter external Vref, 0 - 155 = (0 - 2V"));
            
            v_ref = get_user_int(NULL, 155);
        }
        while(v_ref > 155);
    }
    
    motor_shield.set_current_regulation_mode((Maxrefdes89::max14871_motor_driver_t) md,(Maxrefdes89::max14871_current_regulation_mode_t) mode, v_ref);
}


//*********************************************************************
void clear_screen(void)
{
  Serial.print((char) 0x1B);
  Serial.print("[2J"); //clear screen
  Serial.print((char) 0x1B);
  Serial.print("[H"); //move cursor to Home
}

