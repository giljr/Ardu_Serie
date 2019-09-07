/*
  Ardu-Serie#54
  Project : Adafruit Motor Shield v1 & v2
            4 DC Motors or 2 Stepper Motor or 2 Servos -
            1.2A@25v & 3.2A@15v peak
            https://medium.com/jungletronics/adafruit-motor-shield-v1-v2-2fa1430edaf9

  Ino File: _54_Adafruit_v2_03.ino

  Flavour 03  - Use V2

  Date: Nov 2018

  Description: Simply accelerate two DC Motor 5v forward to 2 s
               and turn the motor to the reverse for another 2 s;
               Then repeat :)

  Board: Adafruit Motor Shield v1
  https://learn.adafruit.com/adafruit-motor-shield/overview

  Connections: See Fritzing in Jungletronics:

  Credits: This sketch was written by SparkFun Electronics,
           with lots of help from the Arduino community.
           Visit https://learn.sparkfun.com/ for more tutorials
           Visit http://www.arduino.cc to learn more about Arduino.

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

void setup()
{
  AFMS.begin();                 // Initialize Adafruit Motor Shield Lib
}


void loop()
{

  myMotor->setSpeed(150);       // Set half speed
  myMotor->run(FORWARD);        // Clockwise
  delay(2000);                  // Wait 2 s

  myMotor->setSpeed(150);       // Set half speed
  myMotor->run(BACKWARD);       // Anti-Clockwise
  delay(2000);                  // Wait 2 s
   
  myMotor->run(RELEASE);        // Stop Motor
  delay(500);                   // Wait half second 
}











