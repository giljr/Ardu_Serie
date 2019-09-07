/*  ArduSerie#68
  Project :  28BYJ-48 Stepper Motor-Peak RPM
             RPM Configurations & Industrial Conveyor Belt
  Ino File: _68_28BYJ-48_FullStep_01.ino
  Flavour 01
  Date: Jan 2019
  Description:  Simply accelerates to maximum the motor in Full-Step Mode
        Tests:  Power Supply 5v .5A USB -> #define waiting_time as 2 (seconds) MAX 14.5 RPM
                Power Supply 9v .5A     -> #define waiting_time as 1 (seconds) MAX 29.0 RPM
                                            but heat issue happens:/  
  Board: ULN2003 28BYJ-48 Stepper Motor & Arduino Uno IDE 1.8.5
  Connections: See Fritzing in https://medium.com/jungletronics
  Credits:  This sketch was written by J3
  Based on: https://medium.com/jungletronics/uln2003-28byj-48-stepper-motor-f1cc5357eff
  
  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/
// Full Step

#define stepper PORTB
#define waiting_time 2 // 1 OR 2 secs

void setup() {
  
  for (int z = 8; z < 12; z++) {  // FULL STEP DRIVE
    pinMode(z, OUTPUT);           // pins 8,9,10,11 output for driving the motor

  }
}

void loop() {

  stepper = 0B0011;               // 0x03;

  delay(waiting_time);

  stepper = 0B0110;               // 0x06;

  delay(waiting_time);

  stepper = 0B1100;               // 0x0C;

  delay(waiting_time);
  stepper = 0B1001;               // 0x09;

  delay(waiting_time);

}
