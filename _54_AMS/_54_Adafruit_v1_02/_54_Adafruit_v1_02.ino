/*
  Ardu-Serie#54
  Project : Adafruit Motor Shield v1 & v2
            4 DC Motors or 2 Stepper Motor or 2 Servos -
            1.2A@25v & 3.2A@15v peak
            https://medium.com/jungletronics/adafruit-motor-shield-v1-v2-2fa1430edaf9
  
  Ino File: _54_Adafruit_v1_02.ino
  
  Flavour 02 - Use V1

  Date: Nov 2018

  Description: Simply accelerate two DC Motor 12v forward to 2 s 
               and turn the motor to the reverse; Stop and then repeat
               See J3 Caterpillar-Crawler-Chassis v 1.0— ArduSerie#46
               https://medium.com/jungletronics/meet-j3c3-3383e9254afd

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

#include <AFMotor.h>                 //Include library AF Motor


AF_DCMotor motor3(3);                // Motor 1 Selected
AF_DCMotor motor4(4);                // Motor 1 Selected

unsigned char velocidade1 = 0x00;    // Storage motor1 Speed (8 bits)


void setup()
{


}


void loop()
{
  velocidade1 = 0xFF;
  motor3.setSpeed(velocidade1);  // Set full Speed
  motor3.run(FORWARD);           // Clockwise
  motor4.setSpeed(velocidade1);  // Set full Speed
  motor4.run(FORWARD);           // Clockwise
  delay(2000);                   // Wait 2 seconds

  velocidade1 = 0x00;
  motor3.setSpeed(velocidade1);  // Set zero Speed
  motor3.run(RELEASE);           // Stop Motor
  motor4.setSpeed(velocidade1);  // Set zero Speed
  motor4.run(RELEASE);           // Stop Motor
  delay(10);                     // Wait 10 microseconds


  velocidade1 = 0xFF;
  motor3.setSpeed(velocidade1);  // Set full Speed
  motor3.run(BACKWARD);          // anti-Clockwise
  motor4.setSpeed(velocidade1);  // Set full Speed
  motor4.run(BACKWARD);          // anti-Clockwise
  delay(2000);                   // Wait 2 seconds

  velocidade1 = 0x00;
  motor3.setSpeed(velocidade1); // Set zero Speed
  motor3.run(RELEASE);          // Stop Motor
  motor4.setSpeed(velocidade1); // Set zero Speed
  motor4.run(RELEASE);          // Stop Motor
  delay(10);                    // Wait 10 microseconds

}











