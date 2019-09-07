/*  — Ardu-Serie#53
   Project name: A4988 — Stepper Motor Driver Carrier
                 Allegro’s A4988 — Bipolar Stepper Motor Driver — 2A@35v peak
     
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour I - Working Code 1
     iNO File: _53_A4988_StepperMotor_01.ino
   
   Revision History:
     Oct, 2018
       - Medium webpage:
         https://medium.com/jungletronics/drv8825-high-current-stepper-motor-driver-carrier-b51eead97fe8 
   
   Description: Hi, this is the same code fro DRV 8825. It works well in A4988 too!
                https://medium.com/jungletronics/a4988-stepper-motor-driver-carrier-bc25497a9d9c
                https://medium.com/jungletronics/jaythree-balancing-car-project-part-5-5-2aaa48924428
         
   IDE:                  Arduino 1.8.5 - http://www.arduino.cc/
   Motor:                Mine is a: JK 42 HS34– 0424A, it means: 
                         A = 42 mm ; B = 34 mm; 0.424A/Coil. 
                         HS means High Speed:), 
                         So, this is one NEMA17
   A4988                 https://cdn.sparkfun.com/datasheets/Robotics/A4988-Datasheet.pdf                   
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

#define M0 4
#define M1 5
#define M2 6

int dPause    =  100;

void setup() 
{
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}

/* motor  FULL throttle */

void loop()
{
  delay(10);
  digitalWrite(M0, LOW );
  digitalWrite(M1, LOW  );
  digitalWrite(M2, LOW  );
  digitalWrite( DIR, HIGH );
  stepGo();
}

void stepGo() 
{
  digitalWrite(STEP, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP, LOW);
  delayMicroseconds(dPause);
}
