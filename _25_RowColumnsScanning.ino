
/*
   Project name: #arduSerie-25
     Studing Bitwise Operations
     Flavour III - Simple Game of PONG Frame
     Hex File: _25_RowColumnsScanning.ino
   Revision History:
     20170110:
       - from https://www.arduino.cc/en/Reference/BitwiseAnd
       and https://youtu.be/evmbh2ntqio?list=PLZ8dBTV2_5HSyOXhJ77d-iyt5Z_v_1DPM
   Description:
          This  initial code serves as the master  structure for  assembling pong game;
          Here  we lighting every LED for an instant to test all before playing.
   MCU:                  Arduino 1.8.2 - @16MHz       http://www.arduino.cc/
   Connections:
      See Official youtube channel vids:

   Based on:   Bitwise AND (&), Bitwise OR (|), Bitwise XOR (^)
               from https://www.arduino.cc/en/Reference/BitwiseAnd
               Led Matrix
               From Small Electronic Thingies for Doing All Kinds of Fun Stuff
               http://wiki.edwindertien.nl/doku.php?id=modules:ledmatrix
   Datasheet Atmega328P  8-bit AVR Microcontrollers:
   http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation

*/

// Test program for LED matrix, Simple PONG game
// 8x8 Matrix is connected using a 74HC138 multiplexer
// on PIN 2,3 and 4 controlling the collumns
// The rows are controlled (sourced) by PIN 5-12
//
// In a timer interrupt (timer2, 1kHz) the display routine is
// called, outputting the global imageMem buffer.
//
// one paddle (potentiometer) is connected to Analog input 0
// autoplay can be defined for a static demo.

int i = 0;
int r = 12;
byte imageMem[8] = {                     // display buffer
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

float pos_x = 0;                        // ball position in axis
float pos_y = 0;

int n;                       // display row counter

void setup() {
  for (int z = 2; z < 14; z++) {
    pinMode(z, OUTPUT);     // all pins output for driving the LED matrix
  }



  // intitialize interrupt timer 2
  TCCR2A = 0;               // TC2 Control Register A - no ocr or pwm ; normal operation
  TCCR2B = 4;               // TC2 Control Register b - Clock Select Bit 1<<CS22 | 0<<CS21 | 0<<CS20 //prescale 1:64
  TIMSK2 = 1 << TOIE2;      // TC2 Interrupt Mask Register - TOIE2: Timer/Counter2, Overflow Interrupt Enabled
  TCNT2 = 0x06;             // The counter value (TCNT2) are initialize in 6 decimal; see calculations below:
}

void loop() {

  for (int w = 0 ; w < 8; w++) {
    for (int z = 0; z < 8; z++) {     // Interacting on each LED to test them
      imageMem[7 - w] = 1 << z - i;   // Lighting each one for an instant
      delay(20);
    }
  }
  i++;                               // Controlling collumns
  if (i > 7) i = 0;

}


ISR(TIMER2_OVF_vect) {      // Interrupt Routine:

  TCNT2 = 0x06;
  PORTD &= 0xE3;            // clear PIN 2,3,4
  PORTD |= (n << 2) & 0x1C; // set collumn trough the mux, see:
  PORTD &= 0x1F;                        // clear PIN 5,6,7
  PORTD |= (imageMem[n] << 5) & 0xE0;   // set row data on 5,6,7
  PORTB &= 0xE0;                        // clear PIN 8,9,10,11,12
  PORTB |= (imageMem[n] >> 3) & 0x1F;   // set row data on 8-12
  n++;                      // increase collumn counter
  if (n > 7) {              // count from 0 to 7
    n = 0;                  // next image. Refreshrate = 1000/8 images/sec = 125Hz (8 x ISR)
  }
}
