
/*
  Project name:
     HowTo Control devices with PIDLibrary —  #arduSerie 23
     Flavour V - Controlling motor without LCD
     Hex File: _23_arduSerie_ArduTachoMeterV2_V.ino
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
/*
Exploring Arduino - Code Listing 4-2: Adjustable Speed Control
http://www.exploringarduino.com/content/ch4

Copyright 2013 Jeremy Blum ( http://www.jeremyblum.com )
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License v3 as published by
the Free Software Foundation.
*/

//Motor Speed Control with a Pot

const int MOTOR=9;  //Motor on Digital Pin 9
const int POT=0;    //POT on Analog Pin 0

int val = 0;

void setup()
{
    pinMode (MOTOR, OUTPUT);
}

void loop()
{
    val = analogRead(POT);
    val = map(val, 0, 1023, 0, 255);
    analogWrite(MOTOR, val);
}

