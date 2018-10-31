/*
   Project name: DRV8825 - High Current Stepper Motor Driver Carrier
                 1 Stepper Motor - Bipolar Mode - 1.5A@45v peak - Ardu_Serie #59
     
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour I - Working Code 1
     iNO File: _59_DRV8825_StepperMotor_01.ino
   
   Revision History:
     Oct, 2018
       - Medium webpage:
         https://medium.com/jungletronics/drv8825-high-current-stepper-motor-driver-carrier-b51eead97fe8 
   
   Description: Hi, this is a higher-performance drop-in replacement for A4988 
                stepper motor driver carriers boards in many applications.
                Simple to use and it is quite robust. 
                I used this driver in my car balance project.
                https://medium.com/jungletronics/jaythree-balancing-car-project-part-5-5-2aaa48924428
         
   IDE:                  Arduino 1.8.5 - http://www.arduino.cc/
   Motor:                5V & 12V 28BYJ-48  Stepper Motor
   https://www.dx.com/p/dmdg-uln2003-stepper-motor-driver-module-5v-28byj-48-stepper-motor-for-arduino-349659#.W9eFWZNKjIU
                         
   DRV8825               Step-Down Stepper Motor Driver Carrier Reprap 4-Layer PCB Set - Purple
                         http://www.dx.com/s/drv8825
   Connections:          See Official youtube channel vids: https://www.youtube.com/user/Giljr2012/
   Datasheet Atmega328P: 8-bit AVR Microcontrollers:
   http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation - There is no guarantee whatsoever. Use at your own risk!
*/

#define DIR 2
#define STEP 3
#define BUTTON A0

#define M0 4
#define M1 5
#define M2 6


int dPause    =  100;
int dirLevel  = HIGH;


void setup() {

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  pinMode( BUTTON, INPUT );

}

/* motor  FULL throttle */

void loop()
{

  delay(10);
  digitalWrite(M0, LOW );
  digitalWrite(M1, LOW  );
  digitalWrite(M2, LOW  );


  digitalWrite( DIR, dirLevel );
  stepGo();
}

void stepGo() {

  digitalWrite(STEP, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP, LOW);
  delayMicroseconds(dPause);

}
