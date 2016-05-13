/*
  Project name:
     06 # arduSerie - Expanding Arduino's Pin w/ DEMUX IC 4051 !!   
     (The 'Hello World' example for the IC 4051)
     Flavour IV
     Hex File: _06_arduSerie_sketch_04.ino
   Revision History:
     20160511:
       - found on internet (gilj3)
      http://colab.lmc.gatech.edu/wp-content/uploads/2012/04/Analog-Mux-4051.pdf
      Description:
      We show Knight Rider LED Light Bars. Knight Riderz LED Light Bars for you!!!
      Pretty simple, just to learn the basics. Easy easy!!!
  MCU:              Arduino - @16MHz   http://www.arduino.cc/
  IC Chip:          CD4051BE           http://pdf1.alldatasheet.com/datasheet-pdf/view/173652/UTC/4051.html
  Connections:
   See google ++ slides:    https://plus.google.com/+gilj3/posts/eJ9dpJnqT5T
     &       you.tube vid:  https://youtu.be/hYpaAO2CYog
   Based on tutorial  http://www.mah.se/k3 (edited)
  
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation.
*/
/*
   codeexample for useing a 4051 * analog multiplexer / demultiplexer
   by david c. and tomek n.* for k3 / malmö högskola

*/
int route = A0; //just a route
int r0 = 0; //value select pin at the 4051 (s0)
int r1 = 0; //value select pin at the 4051 (s1)
int r2 = 0; //value select pin at the 4051 (s2)
int row = 0; // storeing the bin code
int count = 0; // just a count
int bin [] = {000, 1, 10, 11, 100, 101, 110, 111};//bin = binär, some times it is so easy
void setup() {

  pinMode(4, OUTPUT); // s0
  pinMode(5, OUTPUT); // s1
  pinMode(6, OUTPUT); // s2
  // digitalWrite(led, HIGH);
  Serial.begin(9600);
}
void loop () {

  for (count = 0; count <= 7; count++) {
    row = bin[count];
    r0 = row & 0x01;
    r1 = (row >> 1) & 0x01;
    r2 = (row >> 2) & 0x01;
    digitalWrite(4, r0);
    digitalWrite(5, r1);
    digitalWrite(6, r2);
    analogWrite(route, 255); // different way of using analogWrite specifying pin
    delay(100);
  }
  for (count = 7; count >= 0; count--) {
    row = bin[count];
    r0 = row & 0x01;
    r1 = (row >> 1) & 0x01;
    r2 = (row >> 2) & 0x01;
    digitalWrite(4, r0);
    digitalWrite(5, r1);
    digitalWrite(6, r2);
    analogWrite(route, 255); // different way of using analogWrite specifying pin
    delay(100);
  }
}
