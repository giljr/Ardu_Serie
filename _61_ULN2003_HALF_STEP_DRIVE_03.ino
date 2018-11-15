/*   Ardu-Serie#61

  ULN2003 & 28BYJ-48 Stepper Motor
  7-ch Darlington Sink Driver - .6A@50v peak
  https://medium.com/jungletronics/uln2003-28byj-48-stepper-motor-f1cc5357eff

  Ino File: _61_ULN2003_HALF_STEP_DRIVE_03.ino

  Flavour 03

  Date: Nov 2018

  What Are The Ways I Can Run Stepper Motors?
  There are 3: Wave Drive Full Step Half Step

  Description: In Half Step drive we're actually going to sort of make a combination of 
               wave driving & full stepping; this is going to give us a little bit less 
               torque than full stepping because half of the time two phases will 
               be energized but half of the time only one phase will be energized 
               so it's not going to have as much torque as full step; the benefit 
               of half-stepping though is that we're going to get half the step 
               angle so we get double the precision if you will with half stepping;

  Board: 28BYJ-48 Arduino ULN2003 Driver Test Module Board With 5V Stepper Motor
  https://www.amazon.com/Diymore-Stepper-28BYJ-48-Module-ULN2003/dp/B075QCPGG4/ref=sr_1_7?ie=UTF8&qid=1542290588&sr=8-7&keywords=28BYJ-48+5V
  Stepper Motor : 28BYJ-48
  https://www.amazon.com/SODIAL-28BYJ-48-5V-Phase-Stepper-Motor/dp/B00HV3MJXK/ref=sr_1_6?ie=UTF8&qid=1542290588&sr=8-6&keywords=28BYJ-48+5V
  https://www.amazon.com/Vipe-28BYJ-48-Arduino-ULN2003-Stepper/dp/B01N2XYLIV/ref=sr_1_15?s=electronics&ie=UTF8&qid=1542290153&sr=1-15&keywords=28BYJ-48+ULN2003+5V+Stepper+Motor+%2B+ULN2003+Driver

  Connections: See Fritzing in Jungletronics:
  https://medium.com/jungletronics/uln2003-28byj-48-stepper-motor-f1cc5357eff

  Credits: This sketch was written by j3 from http://medium.com/jungletronics

  Based on: https://medium.com/jungletronics/uln2003-28byj-48-stepper-motor-f1cc5357eff

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/
#define stepper PORTB
#define waiting_time 500

void setup() {
  for (int z = 8; z < 12; z++) {

    pinMode(z, OUTPUT);             // pins 8,9,10,11 output for driving the motor

  }
}

void loop() {                       // Half Step Drive

  stepper = 0B0001;                 // 0x01;
  delay(waiting_time);

  stepper = 0B0011;                 // 0x03;
  delay(waiting_time);

  stepper = 0B0010;                 // 0x02;
  delay(waiting_time);

  stepper = 0B0110;                 // 0x06;
  delay(waiting_time);
  
  stepper = 0B0100;                 // 0x04;
  delay(waiting_time);

  stepper = 0B1100;                 // 0x0C;
  delay(500);

  stepper = 0B0001;                 // 0x01;
  delay(waiting_time);

  stepper = 0B1001;                  // 0x09;
  delay(waiting_time);
  
}
