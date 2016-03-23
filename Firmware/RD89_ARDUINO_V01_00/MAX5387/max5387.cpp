/******************************************************************//**
* @file max5387.cpp
*
* @author Justin Jordan
*
* @version 0.0
*
* Started: 18JUL15
*
* Updated: 
* 
* 10DEC15 - ported to Arduino
*
* @brief Source file for Max5387 class
***********************************************************************
* Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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


#include "max5387.h"


#define MAX5387_CMD_A       (0x11)
#define MAX5387_CMD_B       (0x12)
#define MAX5387_CMD_AB      (0x13)
#define MAX5387_DATA_LENGTH (2)
  

//*********************************************************************
Max5387::Max5387()
{
  
}


//*********************************************************************
void Max5387::begin(max5387_i2c_adrs_t i2c_adrs)
{
  _i2c_adrs = i2c_adrs;
  Wire.begin();
}
    

//*********************************************************************  
void Max5387::end()
{
  Wire.end();
}
    

//********************************************************************* 
void Max5387::write_ch_A(uint8_t val)
{
    uint8_t data[] = {MAX5387_CMD_A, val};
    write_data(data);
}
    

//********************************************************************* 
void Max5387::write_ch_B(uint8_t val)
{
    uint8_t data[] = {MAX5387_CMD_B, val};
    write_data(data);
}
    
    
//********************************************************************* 
void Max5387::write_ch_AB(uint8_t val)
{
    uint8_t data[] = {MAX5387_CMD_AB, val};
    write_data(data);
}


//********************************************************************* 
void Max5387::write_data(uint8_t *data)
{
    Wire.beginTransmission(_i2c_adrs);
    Wire.write(data, MAX5387_DATA_LENGTH);
    Wire.endTransmission();
}
    
