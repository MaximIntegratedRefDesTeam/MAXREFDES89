/******************************************************************//**
* @file max5387.h
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
* @brief Header file for Max5387 class
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


#ifndef MAX5387_H
#define MAX5387_H

#include "Arduino.h"
#include "Wire.h"


class Max5387
{
    public:
    
    typedef enum
    {
        MAX5387_I2C_ADRS0 = 0x28,
        MAX5387_I2C_ADRS1,
        MAX5387_I2C_ADRS2,
        MAX5387_I2C_ADRS3,
        MAX5387_I2C_ADRS4,
        MAX5387_I2C_ADRS5,
        MAX5387_I2C_ADRS6,
        MAX5387_I2C_ADRS7
    }max5387_i2c_adrs_t;


	/**********************************************************//**
    * @brief Constructor for Max5387 Class.  
    *
    * On Entry:
    *
    * On Exit:
    *    @return none
    **************************************************************/
    Max5387();

    
    /**********************************************************//**
    * @brief begin function for Max5387 Class.  Use in setup loop 
    *
    * On Entry:
    *     @param[in] i2c_adrs - 7-bit slave address of MAX5387
    *
    * On Exit:
    *    @return none
    **************************************************************/
    void begin(max5387_i2c_adrs_t i2c_adrs);
    
    
    /**********************************************************//**
    * @brief Default destructor for Max5387 Class.  
    *
    * On Entry:
    *
    * On Exit:
    *    @return none
    **************************************************************/
    void end();
    
    
    /**********************************************************//**
    * @brief Write channel A
    *
    * @details    
    *
    * On Entry:
    *    @param[in] val - wiper position of channel A
    *
    * On Exit:
    *    @return none
    **************************************************************/
    void write_ch_A(uint8_t val);
    
    
    /**********************************************************//**
    * @brief Write channel B
    *
    * @details    
    *
    * On Entry:
    *    @param[in] val - wiper position of channel B
    *
    * On Exit:
    *    @return none
    **************************************************************/
    void write_ch_B(uint8_t val);
    
    
    /**********************************************************//**
    * @brief Write channel A&B
    *
    * @details    
    *
    * On Entry:
    *    @param[in] val - wiper position of channel A&B
    *
    * On Exit:
    *    @return none
    **************************************************************/
    void write_ch_AB(uint8_t val);

    private:
    
      max5387_i2c_adrs_t _i2c_adrs;

      void write_data(uint8_t *data);
    
};

#endif/* MAX5387_H */
