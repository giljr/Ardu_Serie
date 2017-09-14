/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour VI - Working with gyroelerometer fo GY-85 Board
     Hex File: _35_GY85_ITG3205_Gyroscope_07.ino
   Revision History:
     Sept, 2017 
       v 1.0 - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
       v 1.1 - Remove Wire.beginTransmission() and Wire.endTransmission() cause it is 
               encapsulated by Wire.requestFrom() - Thanks to https://github.com/Koepel
   Description:
         In this code we initialize the ITG3205 at address 0x69 and proceed with the chip configurations 
         and get the values of axis x, y and z in raw data. This code is very concise and easy to understand. 
         Try youself!
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   IMU:                  9-DOF IMU module - GY-85
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
   Credit:               http://www.geeetech.com/wiki/index.php/ADXL345_Triple_Axis_gyroelerometer_Breakout   
                         http://www.varesano.net//                   
   Data Sheet ITG3205:   http://www.datasheetspdf.com/PDF/ITG3205/731955/1
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is No Warranty. Use at your own risk!
*/
#include <Wire.h>                               // I2C library, gyroscope
//-------- REGISTER MAP for ADXL345 chip   -----// Gyroscope MPU3050 / ITG3200 / ITG3205 
#define GYRO 0x68                               // when AD0 is connected to GND ,gyro address is 0x68; GY-85
//#define GYRO 0x69                             // when AD0 is connected to VCC ,gyro address is 0x69  
#define PWR_MGM     (0x3E)                      // Register 3E – Power Management Control
                                                //   used to manage the power control, select the clock source, 
                                                //   and to issue a master reset to the device - pg 27
#define SMPLRT_DIV  (0x15)                      // Register 21 – Sample Rate Divider
                                                //   determines the sample rate of the ITG-3205 gyros
#define DLPF_FS     (0x16)                      // Register 22 – DLPF, Full Scale
                                                //   configures several parameters related to the sensor acquisition.
                                                //   The FS_SEL parameter allows setting the full-scale range of the gyro sensors
                                                //   The power-on-reset value of FS_SEL is 00h. Set to 03h for proper operation.
#define INT_CFG     (0x17)                      // Register 23 – Interrupt Configuration
                                                //   configures the interrupt operation of the device.  
#define G_TO_READ   (8)                         // 2 bytes for each axis x, y, z
#define TEMP_OUT_H  (0x1B)                      // 16-bit temperature data (2’s complement format)
                                                // https://github.com/xioTechnologies/Serial-Oscilloscope 
int g_offx = -15;                               // Off-Set are Board specific - Use Serial Oscilloscope program above
int g_offy = -35;                               // do it by trial and error; modify the signal 
int g_offz = -35;                               // and write down until you can make zero all the axes
int hx, hy, hz, turetemp;

//---- initializes the gyroscope -  ITG3205 ----//
void initGyro()
{
 /*PWR_MGM Parameters:
   H_RESET Reset device and internal registers to the power-up-default settings
   SLEEP   Enable low power sleep mode
   STBY_XG Put gyro X in standby mode (1=standby, 0=normal)
   STBY_YG Put gyro Y in standby mode (1=standby, 0=normal)
   STBY_ZG Put gyro Z in standby mode (1=standby, 0=normal)
   CLK_SEL Select device clock source

   Register 62 – Power Management - pg 27
   [B7]    [B6]   [B5]       [B4]      [B3]      [B2  B1   B0]
   H_RESET SLEEP  STBY_XG    STBY_YG   STBY_ZG   CLK_SEL       */
   writeTo(GYRO, PWR_MGM,    0B00000000);       //  int osc, no rst, no sleep, normal mode
 /*SMPLRT_DIV Formula:
   The gyros outputs are sampled internally at either 1kHz or 8kHz, determined by the DLPF_CFG
   Fsample = Finternal / (divider+1), where Finternal is either 1kHz or 8kHz
   Sample rate divider: 0 to 255

   Register 21 – Sample Rate Divider - pg 23
   [B7    B6   B5   B4   B3   B2  B1   B0]
   SMPLRT_DIV ***                                */
   writeTo(GYRO, SMPLRT_DIV, 0B00000111);        // divider is 7d
 /*DLPF_FS Parameters:
   FS_SEL   Full scale selection for gyro sensor data
   DLPF_CFG Digital low pass filter configuration and internal sampling rate configuration
   
   Register 22 – DLPF, Full Scale - pg 24
   [B7    B6   B5   [B4   B3]   [B2  B1   B0]
    -      -   -    FS_SEL**    DLPF_CFG**      */
   writeTo(GYRO, DLPF_FS,    0B00011110);       // +/-2000 DPS, DLPF=5Hz, Int.sampleRate=1KHz,
  /*INT_CFG Parameters:
   ACTL             Logic level for INT output pin – 1=active low, 0=active high
   OPEN             Drive type for INT output pin – 1=open drain, 0=push-pull
   LATCH_INT_EN     Latch mode – 1=latch until interrupt is cleared, 0=50us pulse
   INT_ANYRD_2CLEAR Latch clear method – 1=any register read, 0=status register read only
   0                Load zeros into Bit 3 of the Interrupt Configuration register
   ITG_RDY_EN       Enable interrupt when device is ready (PLL ready after changing clock source)
   0                Load zeros into Bit 1 of the Interrupt Configuration register
   RAW_RDY_EN       Enable interrupt when data is available
   
   
   Register 22 – DLPF, Full Scale - pg 24
   [B7]    [B6]   [B5]          [B4]               [B3]   [B2]        [B1]   [B0]
    ACTL   OPEN   LATCH_INT_EN  INT_ ANYRD_2CLEAR   0     ITG_RDY_EN   0     RAW_  RDY_ EN */
   writeTo(GYRO, INT_CFG,    0B00000000);       // logic high, push-pull, no latch, no int
}

//-------- Returns all data Function   ---------//
void getGyroscopeData(int * result)
{
   int regAddress = TEMP_OUT_H;
   int temp, x, y, z;
   byte buff[G_TO_READ];
   readFrom(GYRO, regAddress, G_TO_READ, buff); //read the gyro data from the ITG3200
   result[0] = ((buff[2] << 8) | buff[3]) + g_offx;
   result[1] = ((buff[4] << 8) | buff[5]) + g_offy;
   result[2] = ((buff[6] << 8) | buff[7]) + g_offz;
   result[3] = (buff[0] << 8) | buff[1];        // temperature
 }

//----------------  Set Up   -------------------//
void setup()
{
   Serial.begin(9600);
   Wire.begin();
   initGyro();
}

//----------------    Loop   -------------------//
void loop()
{
   byte addr;
   int gyro[4];
   getGyroscopeData(gyro);
   hx = gyro[0] / 14.375;
   hy = gyro[1] / 14.375;
   hz = gyro[2] / 14.375;
   turetemp = 35+ ((double) (gyro[3] + 13200)) / 280; // temperature
   Serial.print(" X=");
   Serial.print(hx);
   Serial.print(",");
   Serial.print(" Y=");
   Serial.print(hy);
   Serial.print(",");
   Serial.print(" Z=");
   Serial.println(hz);
   //Serial.print(hz);
   //Serial.print(",");
   //Serial.print(" F=");
   //Serial.print(turetemp);
   //Serial.print((char)223);
   //Serial.println("C");
   delay(50); 
}
//---------------- Functions------------------  //
/*Writes val to address register on ACC*/
void writeTo(int DEVICE, byte address, byte val) {
   Wire.beginTransmission(DEVICE);              //start transmission to ACC 
   Wire.write(address);                         // send register address
   Wire.write(val);                             // send value to write
   Wire.endTransmission();                      //end transmission
}
/*reads num bytes starting from address register on ACC in to buff array*/
 void readFrom(int DEVICE, byte address, int num, byte buff[]) {
   Wire.beginTransmission(DEVICE);              //start transmission to ACC 
   Wire.write(address);                         //sends address to read from
   Wire.endTransmission();                      //end transmission
 
   // Wire.beginTransmission(DEVICE);           //start transmission to ACC
   // Removed in Sept 14, 2017: Thanks to https://github.com/Koepel
   // Wire.requestFrom() does a complete I2C transaction on its own
   // Wire.beginTransmission() and Wire.endTransmission() are only used when writing data
   Wire.requestFrom(DEVICE, num);               // request 6 bytes from ACC
 
   int i = 0;
   while(Wire.available())                      //ACC may send less than requested (abnormal)
 { 
   buff[i] = Wire.read();                       // receive a byte
   i++;
 }
   // Wire.endTransmission();                   //end transmission. Perfect Koepel! Thank you very much for your comments!
}
