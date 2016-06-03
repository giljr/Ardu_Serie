/*
  Project name:
     07 # arduSerie - Rotary Encoder Basics For Arduino
     (The 'Hello World' example for the IC 4051)
     Flavour II
     Hex File: _07_arduSerie__RotaryEncoderBasicsWArduino_2.ino
   Revision History:
     20161911:
       - found on internet (gilj3)
     https://youtu.be/HQuLZHsGZdI
      Description:
      Much more sophisticated code. Interrupts each of the four events.
      Use the processing of code together.
      (_07_arduSerie__RotaryEncoderBasicsWArduino_3)
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
//https://www.youtube.com/watch?v=HQuLZHsGZdI
//Kevin Darrah
//
//http://www.kevindarrah.com/download/arduino_code/Encoder.pde

int pulses, A_SIG = 2, B_SIG = 3;

void setup() {
  attachInterrupt(0, A_RISE, RISING);
  attachInterrupt(1, B_RISE, RISING);
  Serial.begin(115200);
}//setup


void loop() {

}

void A_RISE() {
  detachInterrupt(0);
  A_SIG = 1;

  if (B_SIG == 0)
    pulses++;//moving forward
  if (B_SIG == 1)
    pulses--;//moving reverse
  Serial.println(pulses);
  attachInterrupt(0, A_FALL, FALLING);
}

void A_FALL() {
  detachInterrupt(0);
  A_SIG = 0;

  if (B_SIG == 1)
    pulses++;//moving forward
  if (B_SIG == 0)
    pulses--;//moving reverse
  Serial.println(pulses);
  attachInterrupt(0, A_RISE, RISING);
}

void B_RISE() {
  detachInterrupt(1);
  B_SIG = 1;

  if (A_SIG == 1)
    pulses++;//moving forward
  if (A_SIG == 0)
    pulses--;//moving reverse
  Serial.println(pulses);
  attachInterrupt(1, B_FALL, FALLING);
}

void B_FALL() {
  detachInterrupt(1);
  B_SIG = 0;

  if (A_SIG == 0)
    pulses++;//moving forward
  if (A_SIG == 1)
    pulses--;//moving reverse
  Serial.println(pulses);
  attachInterrupt(1, B_RISE, RISING);
}
