/*
  Ardu-Serie#54
  Project : Adafruit Motor Shield v1 & v2
            4 DC Motors or 2 Stepper Motor or 2 Servos -
            1.2A@25v & 3.2A@15v peak
  
  Ino File: _54_Adafruit_v1_01.ino

  SparkFun Motor Driver
  Flavour 01

  Date: Nov 2018

  Description: This is the simplest TB6612FNG board connection on the Arduino.
               The motor accelerates in one direction, then brakes, and then
               accelerates in the opposite direction. I am using a small motor
               and feeding it with 5 volts via VIN pin. Do not use the 5v pin
               to power the motor. Certainly the motor will pull more than 40 mA
               of current and can ruin the Arduino's chip ;/

  Board: TB6612FNG - Driver IC for Dual DC motor
  https://www.sparkfun.com/datasheets/Robotics/TB6612FNG.pdf?_ga=2.166970619.458189902.1538838626-1502777702.1510789918

  Connections: See Fritzing in Jungletronics:
  https://medium.com/jungletronics/tb6612fng-dual-dc-motor-driver-690abd44465d

  Credits: This sketch was written by SparkFun Electronics,
           with lots of help from the Arduino community.
           Visit http://learn.sparkfun.com/products/2 for SIK information.
           Visit http://www.arduino.cc to learn more about Arduino.

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever.
                 Use at your own risk!
*/



#include <AFMotor.h>                 //Include library AF Motor


AF_DCMotor motor1(1);                // Motor 1 Selected


unsigned char velocidade1 = 0x00;    // Storage motor1 Speed (8 bits)


void setup()
{


}


void loop()
{
  velocidade1 = 0xFF;
  motor1.setSpeed(velocidade1);  // Set full Speed
  motor1.run(FORWARD);           // Clockwise
  delay(2000);                   // Wait 2 seconds

  velocidade1 = 0x00;
  motor1.setSpeed(velocidade1);  // Set zero Speed
  motor1.run(RELEASE);           // Stop Motor
  delay(10);                     // Wait 10 microseconds


  velocidade1 = 0xFF;
  motor1.setSpeed(velocidade1);  // Set full Speed
  motor1.run(BACKWARD);          // anti-Clockwise
  delay(2000);                   // Wait 2 seconds

  velocidade1 = 0x00;
  motor1.setSpeed(velocidade1); // Set zero Speed
  motor1.run(RELEASE);          // Stop Motor
  delay(10);                    // Wait 10 microseconds

}











