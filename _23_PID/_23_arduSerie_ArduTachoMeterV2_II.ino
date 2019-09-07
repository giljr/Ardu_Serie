
/*
  Project name:
     HowTo Control devices with PIDLibrary —  #arduSerie 23
     Flavour II - Result Putting Reading in a LCD 16X2
     Hex File: _23_arduSerie_ArduTachoMeterV2_II.ino
   Revision History:
     20161226:
   Description:
       this is the goal: we have to control the speed of an exhaust fan by
       the pid method. We will use the Arduino PID Library by Brett Beauregard
       and Front-End v03 using Processing.org version 3.1.

   MCU:              Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   74HC14            IC Chip - SChmitt Trigger     https://en.wikipedia.org/wiki/Schmitt_trigger
   Connections:
      See Officil you.tube channel vid:
   Based on:   Code by Brett Beauregard

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation

*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,5,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int encoder_pin = 2;                    // pulse output from the module
unsigned int rpm;                       // rpm reading
volatile byte pulses;                   // number of pulses
unsigned long timeold;
                                        // number of pulses per revolution
                                        // based on your encoder disc
unsigned int pulsesperturn = 2;
void counter()
{
                                        //Update count
   pulses++;
}



void setup()
{
   Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  //lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(0,0);
  //lcd.print("Hello, world!!!!");
  //lcd.setCursor(0,1);
  //lcd.print("RPM");

   pinMode(encoder_pin, INPUT);
                                        //Interrupt 0 is digital pin 2
                                        //Triggers on Falling Edge (change from HIGH to LOW)
   attachInterrupt(0, counter, FALLING);
                                        // Initialize
   pulses = 0;
   rpm = 0;
   timeold = 0;
}


void loop()
{
  if (millis() - timeold >= 1000) {
                                        //Don't process interrupts during calculations
      detachInterrupt(0);
      rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses;
      timeold = millis();
      pulses = 0;
      //Serial.print("RPM = ");
      //Serial.println(rpm,DEC);
      //Restart the interrupt processing
      attachInterrupt(0, counter, FALLING);
      lcd.clear();
      lcd.setCursor(0,0);      
      lcd.print(rpm, DEC);
      lcd.setCursor(0,1);
      lcd.print("RPM");
   }
}
