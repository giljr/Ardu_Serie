/*
  Project name:
     07 # arduSerie - Rotary Encoder Basics For Arduino
     (The 'Hello World' example for the IC 4051)
     Flavour I
     Hex File: _07_arduSerie__RotaryEncoderBasicsWArduino_1.ino
   Revision History:
     20161911:
       - found on internet (gilj3)
     https://youtu.be/HQuLZHsGZdI
      Description:
      We unveil the secrets of the rotary encoder and bring the ABCs of this
      simple and powerful component for you without stress.
      Visit our page also detahes. Grateful!
  MCU:              Arduino - @16MHz  http://www.arduino.cc/
  IC Chip:          Rotary Encoder https://en.wikipedia.org/wiki/Rotary_encoder
  Connections:
     you.tube vid:  https://youtu.be/6ok3mH7tmN4
     WebPage: http://jungletronics.blogspot.com.br/2016/05/07-arduserie-rotary-encoder-basics-for.html
   Based on Rotary Encoder Tutorial with Arduino Code
   Code by Kevin Darrah https://youtu.be/HQuLZHsGZdI

   WEB: http://jungletronics.blogspot.com.br/2016/05/07-arduserie-rotary-encoder-basics-for.html
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

//EncodersWArduinoSketch_I.ino
//Interrupt information
// 0 on pin 2
// 1 on pin 3
//code from Cheap Science
//https://www.youtube.com/watch?v=0QLZCfqUeg4
//How to use linear encoders with Arduino. hardware, code, and demo.
//code from Cheap Science
//Encoders with the help of arduino


#define encoderI 2
#define encoderQ 3 // Only use one interrupt in this example

volatile int count;
void setup()
{
  Serial.begin(9600);
  count = 0;
  pinMode(encoderI, INPUT);
  pinMode(encoderQ, INPUT); attachInterrupt(0, handleEncoder, CHANGE);

}

void loop()
{
  Serial.println(count);
  delay(10);
}

void handleEncoder()
{
  if (digitalRead(encoderI) == digitalRead(encoderQ))
  { count++;
  }
  else
  { count--;
  }

}
