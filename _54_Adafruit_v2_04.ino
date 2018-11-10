/*
  Ardu-Serie#54
  Project : Adafruit Motor Shield v1 & v2
            4 DC Motors or 2 Stepper Motor or 2 Servos -
            1.2A@25v & 3.2A@15v peak
            https://medium.com/jungletronics/adafruit-motor-shield-v1-v2-2fa1430edaf9

  Ino File: _54_Adafruit_v2_04.ino

  Flavour 04 - Use V2

  Date: Nov 2018

  Description: Simply accelerate at haff speed two DC Motor 12v
               forward to 2 s and turn the motors back for another 2 s;
               Then repeat :) This is for:
               J3 Caterpillar-Crawler-Chassis v 1.0— ArduSerie#46
               https://medium.com/jungletronics/meet-j3c3-3383e9254afd

  Board: Adafruit Motor Shield v1
  https://learn.adafruit.com/adafruit-motor-shield/overview

  Connections: See Fritzing in Jungletronics:

  Credits: This sketch was written by SparkFun Electronics,
           with lots of help from the Arduino community.
           Visit https://learn.sparkfun.com/ for more tutorials
           Visit http://www.arduino.cc to learn more about Arduino.

  Based on: https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-dc-motors

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *frontMotor = AFMS.getMotor(3);
Adafruit_DCMotor *backMotor = AFMS.getMotor(4);

void setup()
{
  AFMS.begin();                     // Initilize Adafruit Motor Shield
}


void loop()
{

  frontMotor->setSpeed(150);        // Set half Speed
  frontMotor->run(FORWARD);         // Clockwise
  backMotor->setSpeed(150);         // repeat for
  backMotor->run(FORWARD);          // back motor
  delay(2000);                      // Wait 2 seconds

  frontMotor->setSpeed(150);        // Set half Speed
  frontMotor->run(BACKWARD);        // Anti-Clockwise
  backMotor->setSpeed(150);         // repeat for
  backMotor->run(BACKWARD);         // back motor
  delay(2000);                      // Wait 2 seconds

  frontMotor->run(RELEASE);         // Stop front motor
  backMotor->run(RELEASE);          // Stop front motor
  delay(500);                       // Wait half second
}











