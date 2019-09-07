/*
  Ardu-Serie#60
  Project : IFR 520 MOS - Module + DoRobot
            Switch Heavy DC Loads - 10A@100v peak - Ardu-Serie#60
            
  Ino File: _60_IFR520_02.ino

  Flavour 02
  
  Date: Oct 2018

  Description: Use two DC motor and switches the circuit every 2 
              seconds on and off:)                    This is for:
               J3 Caterpillar-Crawler-Chassis v 1.0— ArduSerie#46
               https://medium.com/jungletronics/meet-j3c3-3383e9254afd

  Board: IRF520 MOS FET Driver Module for Arduino / Raspberry Pi
  https://www.dx.com/p/irf520-mos-fet-driver-module-for-arduino-raspberry-pi-red-432012#.W-uDGJNKjIU
  
  Connections: See Fritzing in Jungletronics:

  Credits: This sketch was written by Ahmad Shamshiri for Robojax.com
                      
  Based on: https://youtu.be/17vqLv508Uw

  Terms of Use:  This program is free software: you can redistribute it and/or modify
                 it under the terms of the GNU General Public License v3 as published by
                 the Free Softw are Foundation - There is no guarantee whatsoever :)
                 Use at your own risk!
*/

#define control1 8                  // pin that controls the MOSFET
#define control2 9                  // pin that controls the MOSFET

void setup() {
                                    
  pinMode(control1, OUTPUT);        // define control pin as output
  pinMode(control2, OUTPUT);        // define control pin as output
  Serial.begin(9600);

}

void loop() {
                                    

  digitalWrite(control1, HIGH);     // turn the MOSFET Switch ON
  digitalWrite(control2, HIGH);     // turn the MOSFET Switch ON
  delay(2000);                      // Wait for 2000 ms or 2 second

  digitalWrite(control1, LOW);      // Turn the MOSFET Switch OFF
  digitalWrite(control2, LOW);      // Turn the MOSFET Switch OFF
  delay(2000);                      // Wait for 2000 ms or 2 second


}
