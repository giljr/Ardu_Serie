/*
  Project name:
     Fast Rainbow Circuit #arduSerie 30
     Flavour V - Generate a Graphics from interruptions routine
     Hex File: _30_ATTINY_pwmGraphicGeneration.ino
  
  Revision History:
     March, 2017:

  Description:
     Using interruption to get analogue data to graphic
               
  MCU:                  Arduino 1.8.2 - @16MHz       http://www.arduino.cc/
  MCU:                  ATtiny85  is an 8-bit AVR Microcontroller with 8K ISP flash memory
   
  Connections:
      See Official youtube channel vids: https://goo.gl/mh6TgX
      
  Graphics:
   https://docs.google.com/spreadsheets/d/1HCfaC4OAW71nOij7Lnwlgr5VIIuTOLhIANSV1ZYkAMM/edit?usp=sharing
     
  Based on: Four PWM Outputs from the ATtiny85 by David Johnson-Davies
         http://www.technoblogy.com/show?LE0#cite_note4
                      
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License v3 as published by
  the Free Software Foundation
*/
/* Three PWM Outputs */

// ATtiny85 outputs
#include <FlexiTimer2.h>                // use timer for sampling to get even time base for data

const int READ_A0_PIN = A0;
const int READ_A1_PIN = A1;
const int READ_A2_PIN = A2;



int inverted_value = 0;

void setup() {
  Serial.begin(115200);                 // initialize serial communications (to match that used by PlotNValues.pde)
  pinMode(READ_A0_PIN, INPUT);
  pinMode(READ_A1_PIN, INPUT);
  pinMode(READ_A2_PIN, INPUT);

  FlexiTimer2::set(10, sample);
  FlexiTimer2::start();
}

void loop() {
  //wait interruptions
}

// sample values and pipe to processing
void sample() {
  String result = "";
  Serial.println(result + analogRead(READ_A0_PIN) + "," + analogRead(READ_A1_PIN) + "," + analogRead(READ_A2_PIN));
}
