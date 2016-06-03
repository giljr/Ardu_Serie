/*
  Project name:
     06 # arduSerie - Expanding Arduino's Pin w/ DEMUX IC 4051 !!
     (The 'Hello World' example for the IC 4051)
     Flavour I
     Hex File: _06_arduSerie_sketch_01.ino
   Revision History:
     20160511:
       - found on internet (gilj3)
     http://www.gammon.com.au/forum/?id=11976
      Description:
      Example schematic for reading multiple LDR(light dependent resistors)
      using a single analog port.  We show only the demultiplexing obtained 
      by sensors CdS analog signal in sequence. It is a need to check the output 
      in the serial monitor arduino. Pretty simple, just to learn the basics!!!
  MCU:              Arduino - @16MHz  http://www.arduino.cc/
  IC Chip:          CD4051BE          http://pdf1.alldatasheet.com/datasheet-pdf/view/173652/UTC/4051.html
  Connections:
     See google ++ slides:  https://plus.google.com/+gilj3/posts/eJ9dpJnqT5T
     &       you.tube vid:  https://youtu.be/hYpaAO2CYog
   Based on tutorial 74HC4051 multiplexer / demultiplexer
   Code by Gammon http://www.gammon.com.au/
   WEB: http://jungletronics.blogspot.com.br/2016/05/06-arduserie-expanding-arduinos-pin-w.html
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

// Example of using the 74HC4051 multiplexer/demultiplexer
// Author: Nick Gammon
// Date:   14 March 2013

const byte common = A0;  // where the multiplexer in/out port is connected

// the multiplexer address select lines (A/B/C)
const byte addressA = 6; //better 4, low-order bit
const byte addressB = 5;
const byte addressC = 4; //better 6, high-order bit

void setup ()
{
  Serial.begin (115200);
  Serial.println ("Starting multiplexer test ...");
  pinMode (addressA, OUTPUT);
  pinMode (addressB, OUTPUT);
  pinMode (addressC, OUTPUT);
}  // end of setup

int readSensor (const byte which)
{
  // select correct MUX channel
  digitalWrite (addressA, (which & 1) ? HIGH : LOW);  // low-order bit
  digitalWrite (addressB, (which & 2) ? HIGH : LOW);
  digitalWrite (addressC, (which & 4) ? HIGH : LOW);  // high-order bit
  // now read the sensor
  return analogRead (common);
}  // end of readSensor

void loop ()
{
  // show all 8 sensor readings
  for (byte i = 0; i < 8; i++)
  {
    Serial.print ("Sensor ");
    Serial.print (i);
    Serial.print (" reads: ");
    Serial.println (readSensor (i));
  }
  delay (1000);
}  // end of loop
