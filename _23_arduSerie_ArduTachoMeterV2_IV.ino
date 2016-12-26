
/*
  Project name:
     HowTo Control devices with PIDLibrary —  #arduSerie 23
     Flavour IV - Controlling motor without potentiometer
     Hex File: _23_arduSerie_ArduTachoMeterV2_IV.ino
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
Exploring Arduino - Code Listing 4-1: Automatic Speed Control
http://www.exploringarduino.com/content/ch4

Copyright 2013 Jeremy Blum ( http://www.jeremyblum.com )
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License v3 as published by
the Free Software Foundation.
*/

//Simple Motor Speed Control Program

const int MOTOR=9;    //Motor on Digital Pin 9

void setup()
{
    pinMode (MOTOR, OUTPUT);
}

void loop()
{
    for (int i=0; i<256; i++)
    {
        analogWrite(MOTOR, i);
        delay(10);
    }
    delay(4000);
    for (int i=255; i>=0; i--)
    {
        analogWrite(MOTOR, i);
        delay(10);
    }
    delay(4000);
}

