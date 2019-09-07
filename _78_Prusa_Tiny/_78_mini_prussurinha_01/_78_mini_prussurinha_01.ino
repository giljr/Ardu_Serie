
/*  ArduSerie#78
  Project :     Mini-Prussirinha Printer
                A Funny Tiny Prusa Printer — Ardu_Serie # 78
  Ino File:     _78_mini_prussurinha_01.ino
  Flavour 01
  Date: Aug 2019
  Description:  It’s a tiny printer; A Device for learning 3D Technology; 
                Let’s see how it works using Arduino Micro, Processing, Inscape Apps. 
                We’ll see G-codes, L293D Motor Driver, and more…
  Board:        DC 5V 2-Phase 4-Wire Micro-step Motor Mini Slider Screw Stepper  & 
                L293D Stepper Motor & Arduino Uno IDE 1.8.9
  Connections:  See Fritzing in https://medium.com/jungletronics/mini-prussirinha-printer-d616dde8ae46
  Power Supply: 5v .5A USB Power Cable right into 5.5mm Jack of Adafruit Board
  Credits:      This sketch was written by J3
  Based on:     TecH BoyS ToyS Video How to make a Homework machine for Students https://youtu.be/0sT3qlGAV_U
  copyright:    This code is public domain, enjoy!
  Terms of Use: This program is free software: you can redistribute it and/or modify
                it under the terms of the GNU General Public License v3 as published by
                the Free Software Foundation - There is no guarantee whatsoever :)
                Use at your own risk!
*/
                            // Motor x-Axis
int Red = 14;               // Firing order for  4-Wire Micro-step Motor
int Brown = 15;             // First Brown coil
int Yellow = 16;            // Then Yellow coil and so on...
int Orange = 10;
                            // Motor y-axis
//  int Red = 2;            // Firing order for 28BYJ-48 Stepper Motor
//  int Brown = 3;          // First Brown coil
//  int Yellow = 4;         // Then Yellow coil and so on...
//  int Orange = 5;

long del = 500000;          // 50.000 (+SPEED) -> 999.000 (-SPEED)// ABOVE this figure the motor will stall:/

void setup() {
  pinMode(Red, OUTPUT);
  pinMode(Brown, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Orange, OUTPUT);

  for (int e = 0; e <= 5; e++) {
    //for (int i = 0; i <= 40; i++) {
    for (int i = 0; i <= 35; i++) {
      phaseOne();
      phaseTwo();
      phaseThree();
      phaseFour();
    }
    //for (int j = 0; j <= 40; j++) {
    for (int j = 0; j <= 35; j++) {
      phaseFour();
      phaseThree();
      phaseTwo();
      phaseOne();

    }
  }
}

void phaseOne() {
  digitalWrite(Red, LOW);
  digitalWrite(Brown, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void phaseTwo() {
  digitalWrite(Red, LOW);
  digitalWrite(Brown, HIGH);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseThree() {
  digitalWrite(Red, HIGH);
  digitalWrite(Brown, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseFour() {
  digitalWrite(Red, HIGH);
  digitalWrite(Brown, LOW);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void motorOff() {
  digitalWrite(Red, LOW);
  digitalWrite(Brown, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, LOW);
}

                            // the loop routine runs over and over again forever:
void loop() {
   for (int e = 0; e <= 5; e++) {
    //for (int i = 0; i <= 40; i++) {
    for (int i = 0; i <= 35; i++) {
      phaseOne();
      phaseTwo();
      phaseThree();
      phaseFour();
    }
    //for (int j = 0; j <= 40; j++) {
    for (int j = 0; j <= 35; j++) {
      phaseFour();
      phaseThree();
      phaseTwo();
      phaseOne();

    }
  }
 

}
