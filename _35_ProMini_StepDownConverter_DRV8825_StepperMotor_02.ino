/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour II - Working Code 2
     Hex File: _35_ProMini_StepDownConverter_DRV8825_StepperMotor_02.ino
   Revision History:
     Aug, 2017
       - Medium webpage: https://goo.gl/gHFGIi   &  https://goo.gl/rdL4uY
   Description:
         Simple code to put the engine Nema 17 running in full throtte.
         In this code we walked towards a balancing, two-wheeled car that balances itself!
         The series of several text and codes are based on the work of Joop Brokking
         who published an Awesome project name 'Your Arduino Balancing Robot (YABR)'
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   Motor:                12V NEMA 17 Stepper Motor JK42HS34-0424A
                         http://scionelectronics.com/product/jk42hs34-0424a-nema-17-stepper-motor/
   IMU:                  9-DOF IMU module
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Radio Transceiver:    2.4G Wireless Serial Transparent Transceiver Module for Arduino
                         http://www.dx.com/p/2-4g-wireless-serial-transparent-transceiver-module-for-arduino-441995#.WaMOUCiGPIU
   DRV8825               Step-Down Stepper Motor Driver Carrier Reprap 4-Layer PCB Set - Purple
                         http://www.dx.com/s/drv8825
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
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
