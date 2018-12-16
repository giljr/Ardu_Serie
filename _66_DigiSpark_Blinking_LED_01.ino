/*
  Ardu-Serie#66
  Project : Digispark - Easy Installations Procedures
            Faces problems installing the USB stack on win10?

  Ino File: _66_DigiSpark_Blinking_LED_01.ino

  Flavour 01

  Date: Dez 2018

  Description:  Let's say I want to flash Digispark on-board LED


  Board: Digispark

  Arduino IDE: 1.8.8

  Connections: none

  Credits: This sketch was in public domain.

  Based on: Installing Drivers and Programming the DigiSpark ATtiny85 dev boards - Tutorial
            by Brainy-Bits
            https://youtu.be/MmDBvgrYGZs


  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

void setup() {

  pinMode(1, OUTPUT);

}


void loop() {

  digitalWrite(1, HIGH);  // Turn the LED on
  delay(250);


  digitalWrite(1, LOW);  // Turn the LED off
  delay(250);

}
