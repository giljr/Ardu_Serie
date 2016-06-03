/*
  Project name:
     06 # arduSerie - Expanding Arduino's Pin w/ DEMUX IC 4051 !!
     (The 'Hello World' example for the IC 4051)
     Flavour III
     Hex File: _06_arduSerie_sketch_03.ino
   Revision History:
     20160511:
       - found on internet (gilj3)
      https://github.com/ajfisher/arduino-analog-multiplexer
      Description:
      The official means the arduino work with the 4051 (de)multiplexer.
      Compare just how the signal is captured and written in the control pins.
      Here the interest is only didactic, and you can choose the best approach. 
      all work !!! It's up to you !!!
  MCU:              Arduino - @16MHz   http://www.arduino.cc/
  IC Chip:          CD4051BE           http://pdf1.alldatasheet.com/datasheet-pdf/view/173652/UTC/4051.html
  Connections:
   See google ++ slides:    https://plus.google.com/+gilj3/posts/eJ9dpJnqT5T
     &       you.tube vid:  https://youtu.be/hYpaAO2CYog
   Based on tutorial Analog MuxDemux library
   Code by ajfisher https://github.com/ajfisher

   WEB: http://jungletronics.blogspot.com.br/2016/05/06-arduserie-expanding-arduinos-pin-w.html
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation.
*/
/*

   codeexample for useing a 4051 * analog multiplexer / demultiplexer

   by david c. and tomek n.* for k3 / malm� h�gskola



   edited by Ross R.

*/

#define NO_PINS 8 //how many output pins

int r0 = 0;      //value of select pin at the 4051 (s0)

int r1 = 0;      //value of select pin at the 4051 (s1)

int r2 = 0;      //value of select pin at the 4051 (s2)

int common = A0;   //which y pin we are selecting



void setup() {

  pinMode(4, OUTPUT);    // s0

  pinMode(5, OUTPUT);    // s1

  pinMode(6, OUTPUT);    // s2

}



void loop () {



  for (int count = 0; count <= 8; count++) {

    // select the bit

    r0 = bitRead(count, 0);   // use this with arduino 0013 (and newer versions)

    r1 = bitRead(count, 1);   // use this with arduino 0013 (and newer versions)

    r2 = bitRead(count, 2);   // use this with arduino 0013 (and newer versions)


    //r0 = count & 0x01;      // old version of setting the bits

    //r1 = (count>>1) & 0x01;      // old version of setting the bits

    //r2 = (count>>2) & 0x01;      // old version of setting the bits

    digitalWrite(4, r0);

    digitalWrite(5, r1);

    digitalWrite(6, r2);

    //Either read or write the multiplexed pin here
    analogWrite(common, 255);

    delay(100);

  }

}
