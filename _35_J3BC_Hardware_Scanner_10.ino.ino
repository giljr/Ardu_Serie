/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour VI - Working with gyroelerometer fo GY-85 Board
     Hex File: _35_J3BC_Hardware_Scanner_10.ino
   Revision History:
     Sept, 2017
       - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         This code scans all devices connected to the i2c bus. Serves to test the J3BC v1.0 project
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   Nunchuk Controller:   https://goo.gl/6T3Dvs
   IMU:                  GY-85 Board - 9-DOF IMU module
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on Lib:         Code by Gabriel Bianconi, https://github.com/GabrielBianconi/arduino-nunchuk
   Credit:               Arduino Wii Nunchuck Tutorial https://youtu.be/vhJRR_7m6z4
   WiiChuck Adapter:     https://www.sparkfun.com/products/9281
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is No Warranty. Use at your own risk!
*/
#include <Wire.h>

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);
  Serial.println("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices = 0;
  boolean Acc = 0, Gyr = 0, Nunc = 0;


  Serial.println("\nScanning @ each 2 second...\n");


  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();


    if (error == 0)
    {
      if (address == 0x53)
      {
        Serial.println("1 x ADXL345 Accelerometer detected!");
        Acc = 1;
        nDevices++;
      }
      if (address == 0x68)
      {
        Serial.println("1 x ITG3205 Giroscope     detected!");
        Gyr = 1;
        nDevices++;
      }
      if (address == 0x1E)
      {
        Serial.println("1 x HMC5883L Magnetometer detected!");
        nDevices++;
      }
      if (address == 0x52)
      {
        Serial.println("1 x Wiichuck              detected!");
        Nunc = 1;
        nDevices++;
      }
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");

  if (!Nunc) {
    Serial.print("\nYou do not have:  ");
    Serial.println("1 x Wiichuck");
    Serial.println("Check it out, please!");
  }
  if (!Acc) {
    Serial.print("\nYou do not have:  ");
    Serial.println("1 x Accelerometer");
    Serial.println("Check it out, please!");
  }
  if (!Gyr) {
    Serial.print("\nYou do not have:  ");
    Serial.println("1 x Giroscope");
    Serial.println("Check it out, please!");
  }
  else if (Acc == 1 && Gyr == 1 && Nunc == 1) {
    Serial.println("\nCongratulations:");
    Serial.println("You have the necessary I2C hardware for JayThree Balancing Car Project!");
  }

  Serial.println("\nActually,we need this:");
  Serial.println("1 x Accelerometer");
  Serial.println("1 x Giroscope");
  Serial.println("1 x Wiichuck");

  delay(20000);           // wait 5 seconds for next scan

}

