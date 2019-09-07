/*  Ardu-Serie#52
   Project name: L298N - Dual Full-Bridge Driver
                 Darlington Transistor Arrays Based - 3A@50v peak

     Flavour I - 1 Motor @5v
     iNO File: _52_L298N_DualDriver_01.ino

   Revision History:
     Oct, 2018
       - Medium webpage:


   Description: This is the simplest program possible. We connected a simple 5v DC motor.


   IDE:                  Arduino 1.8.5 - http://www.arduino.cc/
   Motor:                5V

   L298N                 Step-Down Stepper Motor Driver Carrier Reprap 4-Layer PCB Set - Purple
                         http://www.dx.com/s/drv8825
   Connections:          See Official youtube channel vids: https://www.youtube.com/user/Giljr2012/
   Datasheet L298N:
   http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is no guarantee whatsoever. Use at your own risk!
*/

#define IN1 6
#define IN2 7
#define ENA 9

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
                          // Set initial direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

}

void loop() {


}
