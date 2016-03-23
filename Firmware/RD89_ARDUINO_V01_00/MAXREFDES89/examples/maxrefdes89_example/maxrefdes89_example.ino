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
  
  //config motor driver driven by pwm signal on D5 to spin motor in CW direction, depending on wirring
  motor_shield.set_operating_mode(Maxrefdes89::MD2, Maxrefdes89::FORWARD);
}


void loop() 
{
  //ramp motor up
  for(idx = 0; idx < 255; idx += 5)
  {
    motor_shield.set_pwm_duty_cycle(Maxrefdes89::MD2, idx);
    Serial.print("idx = ");
    Serial.println(idx, DEC);
    delay(100);
  }

  //ramp motor down
  for(idx = 255; idx > 0; idx -= 5)
  {
    motor_shield.set_pwm_duty_cycle(Maxrefdes89::MD2, idx);
    Serial.print("idx = ");
    Serial.println(idx, DEC);
    delay(100);
  }
}

