/*  
  Ardu-Serie#49
  Project : TB6612FNG - Dual DC Motor Driver
           SparkFun Motor Driver - 3.2A@13.5v peak
  
  Ino File: _49_TB6612FNG_Lab_02.ino

  SparkFun Motor Driver
  Flavour 02

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
                        //define the two direction logic pins and the speed / PWMA and PWMB pin
const int PWMA = 6;     // Front Motor (A)
const int AIN1 = 5;
const int AIN2 = 4;
                        // Rear  Motor (B)
const int PWMB = 3;
const int BIN1 = 2;
const int BIN2 = 1;

void setup()
{
                        // Motor A : set all pins as output
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

                        // Motor B : set all pins as output
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

void loop()
{
                        //drive forward at full speed by pulling AIN1/BIN1 High
                        //and AIN2/BIN2 low, while writing a full 255 to PWMA/PWMB to
                        //control speed
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 255);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 255);

                        //wait 1 second
  delay(1000);

                        //Brake the motorS by pulling both direction pins to
                        //the same state (in this case LOW). PWMA/PWMB doesn't matter
                        //in a brake situation, but set as 0.
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);

                        //wait 1 second
  delay(1000);

                        //change direction to reverse by flipping the states
                        //of the direction pins from their forward state at lower speed
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 150);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 150);

                        //wait 1 second
  delay(1000);

                        //Brake again
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);

                        //wait 1 second
  delay(1000);
}
