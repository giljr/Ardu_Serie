/*  Ardu-Serie#52
   Project name: L298N - Dual Full-Bridge Driver
                 Darlington Transistor Arrays Based - 3A@50v peak

     Flavour II - 2 DC Motors @12v
     iNO File: _52_L298N_DualDriver_02.ino

   Revision History:
     Oct, 2018
       - Medium webpage:


   Description: This is the simplest program possible for L298N. We connected a simple 5v DC motor.


   IDE:                  Arduino 1.8.5 - http://www.arduino.cc/
   Motor:                5V

   L298N                 Step-Down Stepper Motor Driver Carrier Reprap 4-Layer PCB Set - Purple
                        
   Connections:          See Official youtube channel vids: https://www.youtube.com/user/Giljr2012/
   Datasheet L298N:
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is no guarantee whatsoever. Use at your own risk!
*/

                  //Port's Commands Motor A
int IN1 = 4;
int IN2 = 5;
int ENA = 9;
                  //Port's Commands Motor B
int IN3 = 6;
int IN4 = 7;
int ENB = 10;

// Vitesse du moteur
int state = 255;

void setup() {
                  // Port's Configurations
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() { 
                  // Go Straight Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, state);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, state);


}
