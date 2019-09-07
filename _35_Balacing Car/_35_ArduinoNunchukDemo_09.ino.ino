/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour VI - Working with gyroelerometer fo GY-85 Board
     Hex File: _35_ArduinoNunchukDemo_09.ino
   Revision History:
     Sept, 2017
       - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         In this code, we will see how to interface a Wii nunchuck to the Arduino (Uno, Pro Mini)
         and how to take measurements in code to determine the position of the joystick,
         accelerometer motions and if the nunchuck buttons are pressed. The Wii nunchuck
         is a great controller for your electronics projects to control external peripherals.
         You'll need the library ArduinoNunchuk.h Link below.
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
#include <Wire.h>
#include <ArduinoNunchuk.h>

#define BAUDRATE 9600

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();
}

void loop()
{
  nunchuk.update();

  Serial.print(nunchuk.analogX, DEC);
  Serial.print(',');
  Serial.print(nunchuk.analogY, DEC);
  Serial.print(',');
  Serial.print(nunchuk.accelX, DEC);
  Serial.print(',');
  Serial.print(nunchuk.accelY, DEC);
  Serial.print(',');
  Serial.print(nunchuk.accelZ, DEC);
  Serial.print(',');
  Serial.print(nunchuk.zButton, DEC);
  Serial.print(',');
  Serial.println(nunchuk.cButton, DEC);

}
