/*
  Project name:
     06 # arduSerie - Expanding Arduino's Pin w/ DEMUX IC 4051 !!   
     (The 'Hello World' example for the IC 4051)
     Flavour II
     Hex File: _06_arduSerie_sketch_02.ino
   Revision History:
     20160511:
       - found on internet (gilj3)
      https://github.com/ajfisher/arduino-analog-multiplexer
   Description:
      This library is used in order to abstract away a few of the
      details to do with analog multiplexing and demultiplexing.
      We show only the demultiplexing transmitting signal to each
      of the 8 LEDs in sequence. Pretty simple, just to learn the 
      basics. Easy easy!!!
   MCU:              Arduino - @16MHz   http://www.arduino.cc/
   IC Chip:          CD4051BE           http://pdf1.alldatasheet.com/datasheet-pdf/view/173652/UTC/4051.html
   Connections:
   See google ++ slides:     https://plus.google.com/+gilj3/posts/eJ9dpJnqT5T
     &       you.tube vid:   https://youtu.be/hYpaAO2CYog
   Based on: tutorial Analog MuxDemux library
   Code by: ajfisher https://github.com/ajfisher
  
   WEB: http://jungletronics.blogspot.com.br/2016/05/06-arduserie-expanding-arduinos-pin-w.html 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/
/**
  Author: Andrew Fisher
  Version: 0.1.3
  Date: 1 April, 2013
  History:
  1 April, 2013 - Minor updates to standardise the code.
  23 September, 2012 - Updates for the change to Demuxer arrangement
  22 September, 2012 - Andrew Fisher, Updates to Arduino 1.0
  May 1, 2011 - Andrew Fisher, Original
  Uses the analog multiplexer library to control some LEDs
  See the fritzing circuit in the code library for set up.
**/

#include "analogmuxdemux.h"

#define WRITEPIN A0 // connect to a PWM pin in order to be able to output an analog signal
#define NO_PINS 8 // how many output pins are you going to use on the DeMux? This exmample uses 3.

// set up the DeMux ready to be used. Watch the order of S0, S1 and S2.
AnalogDeMux admux(4, 5, 6, WRITEPIN);

void setup() {
  pinMode(WRITEPIN, OUTPUT);
  delay(1);
}

void loop() {

  // go through each pin in turn and then just light it up like a dimmer
  for (int pinno = 0; pinno < NO_PINS; pinno++) {

    admux.SelectPin(pinno); // choose the pin you want to send signal to off the DeMux
    for (int i = 0; i < 255; i++) {
      admux.AnalogWrite(i); // simply pass an analog value as per normal.
      delay(1);
    }
    for (int i = 255; i >= 0; i--) {
      admux.AnalogWrite(pinno, i); // different way of using analogWrite specifying pin
      delay(1);
    }
  }
}
