/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour VI - Working with Accelerometer fo GY-85 Board
     Hex File: _35_GY85_ADXL345_Accelerometer_06.ino
   Revision History:
     Sept, 2017
       - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         In this code we initialize the ADXL345 at address 0x53 and proceed with the chip configurations 
         and get the values of axis x, y and z in raw data. This code is very concise and easy to understand. 
         Try youself!
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   IMU:                  9-DOF IMU module
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
   Credit:               http://www.geeetech.com/wiki/index.php/ADXL345_Triple_Axis_Accelerometer_Breakout                      
   Data Sheet ADXL345:   http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is No Warranty. Use at your own risk!
*/

#include <Wire.h>                               // I2C library
//-------- REGISTER MAP for ADXL345 chip   -----// 
#define ACC         (0x53)                      // ADXL345 ACC address
#define POWER_CTL   (0x2D)                      // Power-saving features control 
#define DATA_FORMAT (0x31)                      // Data format control 
#define BW_RATE     (0x2C)                      // Data rate and power mode control 
#define DATAX0      (0x32)                      // First address for axis x

#define A_TO_READ   (6)                         // num of bytes we are going to read each time 
                                                // two bytes for each axis

//-- Initialization - Turning on the ADXL345 ---//
/*by default the device is in +-2g range reading*/
void initAcc() {
  
  /*Register 0x2D—POWER_CTL (Read/Write)
    [D7] [D6] [D5]   [D4]         [D3]      [D2]    [D1  D0]
    0    0    Link   AUTO_SLEEP   Measure*  Sleep   Wakeup    */  
  writeTo(ACC, POWER_CTL,   0B00001000);        // 1 places the part into measurement mode     
  /*Register 0x31—DATA_FORMAT (Read/Write)
    [D7]       [D6]  [D5]       [D4]  [D3]       [D2]     [D1 D0]
    SELF_TEST  SPI   INT_INVERT  0    FULL_RES*  Justify  Range**   */
  writeTo(ACC, DATA_FORMAT, 0B00001011);        // +-16g range, right-justified, full resolution
   /*Register 0x2C—BW_RATE (Read/Write)
    [D7] [D6] [D5] [D4]        [D3 D2 D1 D0]
    0     0    0   LOW_POWER   Rate***          */
  writeTo(ACC, BW_RATE,     0B00101100);        // 1100 -> ODR = 400Hz, 200 bandwidth = ODR/2 
                                                // (ODR = Output Data Rate); Table 7 & 8 DS
                                                // same outputting data at each 10 ms (T = 1/ F)                                              
}

//-------- Returns all data Function   ---------//
void getAccelerometerData(int * result) {

  int regAddress = DATAX0;                      // first axis-acceleration-data register on the ADXL345
  byte buff[A_TO_READ];                         // simultaneous reading as recommended by the data sheet
  readFrom(ACC, regAddress, A_TO_READ, buff);   // read the acceleration data from the ADXL345
                                                // each axis reading comes in 10 bit resolution
                                                // ie 2 bytes. Least Significat Byte first!!
                                                //thus we are converting both bytes in to one int
  result[0] = (((int)buff[1]) << 8) | buff[0];   
  result[1] = (((int)buff[3])<< 8) | buff[2];
  result[2] = (((int)buff[5]) << 8) | buff[4];

}

//----------------  Set Up   -------------------//
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  initAcc();
}

//----------------    Loop   -------------------//
void loop()
{
  int hx,hy,hz;
  int acc[3];
  getAccelerometerData(acc);
  hx = acc[0];
  hy = acc[1];
  hz = acc[2];
  Serial.print(" X=");
  Serial.print(hx);
  //Serial.print(hx/256); //in g
  Serial.print(",");
  Serial.print(" Y=");
  Serial.print(hy);
  //Serial.print(hy/256);//in g
  Serial.print(",");
  Serial.print(" Z=");
  Serial.println(hz);
  //Serial.println(hz/256);//in g
  delay(50); 
}
//---------------- Functions------------------  //
                                                // Writes val to address register on ACC
void writeTo(int DEVICE, byte address, byte val) {
 
  Wire.beginTransmission(DEVICE);               //start transmission to ACC 
  Wire.write(address);                          // send register address
  Wire.write(val);                              // send value to write
  Wire.endTransmission();                       // end transmission
 
}
                                                // reads num bytes starting from address 
                                                // register on ACC in to buff array
void readFrom(int DEVICE, byte address, int num, byte buff[]) {
  
  Wire.beginTransmission(DEVICE);               // start transmission to ACC 
  Wire.write(address);                          // sends address to read from
  Wire.endTransmission();                       // end transmission

  Wire.beginTransmission(DEVICE);               // start transmission to ACC
  Wire.requestFrom(DEVICE, num);                // request 6 bytes from ACC
  int i = 0;
  while(Wire.available())                       // ACC may send less than requested (abnormal)
  { 
    buff[i] = Wire.read();                      // receive a byte
    i++;
  }
  Wire.endTransmission();                       // end transmission.

}
