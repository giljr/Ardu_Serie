/*  ArduSerie#64

  Project :  BTS 7960B- High Current PN Half Bridge 
             High Current Motor Drive Applications - NovalithIC T M 

  Ino File: _64_BTS7960B_01.ino

  Flavour 01

  Date: Oct 2018

  Description:  Simply accelerates to maximum then slows to a stop. then repeat the cycle.
  
  Board: BTS 7960B -High Current PN Half Bridge NovalithIC T M
  https://www.infineon.com/dgdl/bts7960b-pb-final.pdf?folderId=db3a3043156fd5730116144c5d101c30&fileId=db3a30431ed1d7b2011efe782ebd6b60

  Connections: See Fritzing in Jungletronics:

  Credits:  This sketch was written by Mohannad Rawashdeh

  Based on: https://www.instructables.com/member/Mohannad+Rawashdeh/

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

int RPWM1 = 5;                       // Two board used here, 1 
int LPWM1 = 6;                             
int L_EN1 = 7;
int R_EN1 = 8;

int RPWM2 = 9;                       // and 2
int LPWM2 = 10;                              
int L_EN2 = 11;
int R_EN2 = 12;

void setup() {
                                    
  for (int i = 5; i < 12; i++) {     // Configure pins 5,6,7,8,9,10,11 as Output
    pinMode(i, OUTPUT);
  }
  
  for (int i = 5; i < 12; i++) {     // Configure pins 5,6,7,8,9,10,11 as digital LOW signal
    digitalWrite(i, LOW);
  }
  delay(10);
  
  digitalWrite(R_EN1, HIGH);          // Enable all pins
  digitalWrite(L_EN1, HIGH);

  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  
}



void loop() {


  delay(10);

  for (int i = 0; i < 256; i++) {     // accelerates to maximum

    analogWrite(RPWM1, i);
    analogWrite(RPWM2, i);
    //  analogWrite(LPWM1,255-i);
    //  analogWrite(LPWM2,255-i);
    delay(100);
  }
  delay(10);

  for (int i = 255; i > 0; i--) {     //  slows to a stop

    analogWrite(RPWM1, i);
    analogWrite(RPWM2, i);
    // analogWrite(LPWM1,255-i);
    // analogWrite(LPWM2,255-i);
    delay(100);
  }
  delay(10);
}

