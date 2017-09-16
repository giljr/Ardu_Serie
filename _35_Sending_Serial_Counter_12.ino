/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour XII - Working with WiiNunchunk for J3 Balancing Car Project
     Ino File: _35_Sending_Serial_Counter_12.ino
   Revision History:
     Sept, 2017
       - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         Simple code to send counting data to the serial from Arduino UNO.
         Needs other simple code for receiving data in Arduino Mini.
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
int counter = 0;
void setup() {
    Serial.begin(9600);
    delay(50);
}
void loop() {
    counter++;
    Serial.println(counter);
    delay(500); // wait half a sec
}
