/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour III - Working Code 3
     Hex File: _35_ProMini_StepDownConverter_DRV8825_StepperMotor_BatMon_03.ino
   Revision History:
     Aug, 2017
       - Medium webpage: https://goo.gl/gHFGIi   &  https://goo.gl/rdL4uY
   Description:
         In this code we walked towards a balancing, two-wheeled car that balances itself!
         The series of several text and codes are based on the work of Joop Brokking
         who published an Awesome project name 'Your Arduino Balancing Robot (YABR)'
         A part of the code deals with the working level of the 11v1 battery.
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
#define _TRUE 1
#define _FALSE 0


int dPause    =  100;
int dirLevel  = HIGH;

int battery_voltage;
byte low_bat = 0;

/* in production comment debug = _TRUE */

boolean debug = _TRUE;
//boolean debug = _FALSE;


void setup() {

  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  pinMode( BUTTON, INPUT );

  if (debug) {

    Serial.begin(9600);
    Serial.println( "BATT Test: Stop Range level 8v < -- > 10v5" );
    Serial.println( "BATT Output Must be above > 1050" );

  }

}

void loop()
{
  /*
    Load the battery voltage to the battery_voltage variable.
    Resistor voltage divider => (3.3k + 3.3k)/2.2k = 2.5
    12.5V equals ~5V @ Analog 0.
    12.5V equals 1023 analogRead(0).
    1250 / 1023 ~= 1.222
    The variable battery_voltage holds 1050 if the battery voltage is 10.5V.
    DEBUG: Please, to debug batt voltage keep USB TTL cable connected:
    when the Arduino is transmitting the USB TTL adapter will receive.
    If battery_voltage is below 10.5V and higher than 8.0V
    Turn on the led if battery voltage is to low
    Set the low_bat variable to 1 - for future routine
  */
  
  battery_voltage = analogRead(A0) * (1250 / 1023.);

  if (debug) {
    Serial.println(battery_voltage);
    delay(2000);
  }

  if (battery_voltage < 1050 && battery_voltage > 800) {
    digitalWrite(13, HIGH);
    low_bat = 1;
  }

  delay(10);

  /* motor  FULL throttle */

  digitalWrite(M0, LOW );
  digitalWrite(M1, LOW  );
  digitalWrite(M2, LOW  );

  digitalWrite( DIR, dirLevel );

  /* Motor  rotates if there is enough battery power.
    Otherwise the LED 13 lights indicating that the battery is discharged */

  if (!low_bat)
  {
    stepGo();
  }

}

void stepGo() {

  digitalWrite(STEP, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP, LOW);
  delayMicroseconds(dPause);

}
