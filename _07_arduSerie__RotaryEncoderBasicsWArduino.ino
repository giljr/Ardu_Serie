/*
  Project name:
     07 # arduSerie - Rotary Encoder Basics For Arduino
     (The 'Hello World' example for the IC 4051)
     Flavour II
     Hex File: _07_arduSerie__RotaryEncoderBasicsWArduino.ino
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
   Based on Rotary Encoder Tutorial with Arduino Code
   Code by Kevin Darrah https://youtu.be/HQuLZHsGZdI

   WEB: http://jungletronics.blogspot.com.br/2016/05/07-arduserie-rotary-encoder-basics-for.html
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

/*
   EncodersWArduinoSketch_II
  Demonstrates use of rotary encoder for motor
  direction and distance.
  http://www.bristolwatch.com/arduino/arduino2.htm
  Rotary Encoder Using Arduino Hardware Interrupts
  by Lewis Loflin
*/

#define CHA 2
#define CHB 3
#define CW_LED 8
#define CCW_LED 7

volatile int master_count = 0; // universal count
volatile byte INTFLAG1 = 0; // interrupt status flag

void setup() {
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  pinMode(CW_LED, OUTPUT); // LED connected to pin to ground
  pinMode(CCW_LED, OUTPUT); // LED connected to pin to ground

  Serial.begin(115200);
  Serial.println(master_count);

  attachInterrupt(0, flag, RISING);
  // interrupt 0 digital pin 2 positive edge trigger
}

void loop() {

  if (INTFLAG1)   {
    Serial.println(master_count);
    delay(500);
    INTFLAG1 = 0; // clear flag
  } // end if


} // end loop


void flag() {
  INTFLAG1 = 1;
  // add 1 to count for CW
  if (digitalRead(CHA) && !digitalRead(CHB)) {
    master_count++ ;
    digitalWrite(CW_LED, HIGH);
    digitalWrite(CCW_LED, LOW);
  }
  // subtract 1 from count for CCW
  if (digitalRead(CHA) && digitalRead(CHB)) {
    master_count-- ;
    digitalWrite(CW_LED, LOW);
    digitalWrite(CCW_LED, HIGH);
  }
}
