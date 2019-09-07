/*  ArduSerie#68
  Project :     228BYJ-48 5v Stepper Motor - How many RPM Can I Spin This Thing?
                RPM Configurations & Industrial Conveyor Belt - ArduSerie#68
  Ino File:     _68_L293D_DualHBridge_03.ino
  Flavour 03
  Date: Jan 2019
  Description:  I drew a circuit with L293D Motor Drive DIP 16 Pin IC; the code simply makes 
                the motor spin at 24 RPM without stopping. Basically the code
                I removed the chip from the Adafruit board. It was an emergency, 
                I already provided the online chip :)…
  Board:       L293D Motor Drive DIP 16 Pin IC & 28BYJ-48 Stepper Motor & Arduino Uno IDE 1.8.5
  Connections:  See Fritzing in https://medium.com/jungletronics/28byj-48-stepper-motor-peak-rpm-658eae6afe2f
  Credits:      This sketch was written by Adafruit and modifeid by J3
  Based on:     https://medium.com/jungletronics/uln2003-28byj-48-stepper-motor-f1cc5357eff
  copyright:    Adafruit Industries LLC, 2009
                this code is public domain, enjoy!
  Terms of Use: This program is free software: you can redistribute it and/or modify
                it under the terms of the GNU General Public License v3 as published by
                the Free Softw are Foundation - There is no guarantee whatsoever :)
                Use at your own risk!
*/

int Blue = 0;               // Firing order for 28BYJ-48 Stepper Motor
int Pink = 1;               // First Blue coil
int Yellow = 2;             // Then yellow coil and so on...
int Orange = 3;
long del = 1200;            // Below this figure the motor will stall:/

void setup() {
  pinMode(Blue, OUTPUT);
  pinMode(Pink, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Orange, OUTPUT);
}

void phaseOne() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void phaseTwo() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, HIGH);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseThree() {
  digitalWrite(Blue, HIGH);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseFour() {
  digitalWrite(Blue, HIGH);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void motorOff() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, LOW);
}

// the loop routine runs over and over again forever:
void loop() {

  for (int i = 0; i <= 500; i++) {
    phaseOne();
    phaseTwo();
    phaseThree();
    phaseFour();
  }

}

