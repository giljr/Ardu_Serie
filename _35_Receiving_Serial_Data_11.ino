/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour VI - Working with gyroelerometer fo GY-85 Board
     Hex File: _35_ArduinoNunchukDemo_09.ino
   Revision History:
     Sept, 2017
       - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         Simple code to receive serial data. 
Simple code to send nunch data to the serial
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   Nunchuk Controller:   https://goo.gl/6T3Dvs
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on Lib:         Code by Gabriel Bianconi, https://github.com/GabrielBianconi/arduino-nunchuk
   Credit:               Arduino Wii Nunchuck Tutorial https://youtu.be/vhJRR_7m6z4
   WiiChuck Adapter:     https://www.sparkfun.com/products/9281
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is No Warranty. Use at your own risk!
*/
char data;

void setup() {
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0) 
  {
    data = Serial.read();
    Serial.print(data);
  }


}
