/*
  Project name:
     15 # arduSerie - Arduino Ripple Board w/ PCF8591P
     (auxiliary code in C ++ for implementation of raspberry ripple board)
     Flavour III - Reading Analog Channels
     Hex File: _15_arduSerie_sketch_03_PCF8591_readAnalog.ino
   Revision History:
     20160909:
       - board found on Raspberry Pi for Dummies Book - Sean McManus & Mike Cook
         http://goo.gl/swCq7E
   Description:
      Reads the four analog inputs PCF8591 and shows in the serial port.
      You can put any sensor in these ports, such as 4 ldr, 4 tmp35
      or 4 potentiometers or combinations of these components.
      Pretty simple, just to learn the basics of PCF8591P chip!!!
      Address: 000

   MCU:              Arduino - @16MHz       http://www.arduino.cc/
   MCU:              Raspberry Pi - Model B https://www.raspberrypi.org/
   IC Chip:          PCF8591P               http://goo.gl/h1HMRK

   Connections:
      See google ++ slides:
      & Officil you.tube channel vid:
   
   Based on:   tutorial tronixstuff.com http://goo.gl/G2GJFL Chapter 52
               Code by John Boxall June 2013

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation

*/

#include "Wire.h"
//Bit shift of the bus address as Arduino sends 7-bit addresses but the PCF8591 wants an 8-bit,
//so we must shift the byte over by one bit. 
//Choose the correct addresses pinout of your PCF8591 connection; 
//list of 8 options:

#define PCF8591 (0x90>> 1)            //hex 48; decimal 72; binary 1001[000] - I2C bus address
//#define PCF8591 (0x92>> 1)            //hex 49; decimal 73; binary 1001[001] - I2C bus address
//#define PCF8591 (0x94>> 1)              //hex 4A; decimal 74; binary 1001[010] - I2C bus address
//#define PCF8591 (0x96>> 1)            //hex 4B; decimal 75; binary 1001[011] - I2C bus address
//#define PCF8591 (0x98>> 1)            //hex 4C; decimal 76; binary 1001[100] - I2C bus address
//#define PCF8591 (0x9A>> 1)            //hex 4D; decimal 77; binary 1001[101] - I2C bus address
//#define PCF8591 (0x9C>> 1)            //hex 4E; decimal 78; binary 1001[110] - I2C bus address
//#define PCF8591 (0x9F>> 1)            //hex 4F; decimal 79; binary 1001[111] - I2C bus address

byte value0, value1, value2, value3;
void setup()
{
  Wire.begin();
  Serial.begin(9600);
}
void loop()
{
  Wire.beginTransmission(PCF8591);      // wake up PCF8591
  Wire.write(0x04);                     // control byte - read ADC0 then auto-increment
  Wire.endTransmission();               // end tranmission
  Wire.requestFrom(PCF8591, 5);
  value0 = Wire.read();                 // Dummy read
  value0 = Wire.read();                 // ch 0
  value1 = Wire.read();                 // ch 1
  value2 = Wire.read();                 // ch 2
  value3 = Wire.read();                 // ch 3  
  Serial.print(value0); Serial.print(" ");
  Serial.print(value1); Serial.print(" ");
  Serial.print(value2); Serial.print(" ");
  Serial.print(value3); Serial.print(" ");
  Serial.println();
  //delay(100);
}
