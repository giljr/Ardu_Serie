
/*
   Project name: #arduSerie-25
     Studing Bitwise Operations
     Flavour III - Simple Game of PONG Frame
     Hex File: _25_bitwise_pong_game_00.ino
   Revision History:
     20170110:
       - from https://www.arduino.cc/en/Reference/BitwiseAnd
       and https://youtu.be/evmbh2ntqio?list=PLZ8dBTV2_5HSyOXhJ77d-iyt5Z_v_1DPM
   Description:
          This  initial code serves as the master  structure for  assembling pong game;
          Here  we implement only the interrupt  at each 1ms(1kHz) and  distributes each
          signal of 1ms in 3 pins,counting from 0 to 8,causing a image refresh every 8ms;
          The distribution of the signal in this 3 pins causes it to renew at each 8ms all
          the lights of the matrix 8x8; This works like 16mm Movie Camera. Cool, right?
          (See the next code for complete implementation of the interrupted complete pong
          game using timer 2 and please read the NOTE below).
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

int n;                       // display row counter

void setup() {
  for (int z = 2; z < 14; z++) {
    pinMode(z, OUTPUT);     // all pins output for driving the LED matrix
  }

  //digitalWrite(5, HIGH);     // all pins output for driving the LED matrix
  //digitalWrite(12, HIGH);     // all pins output for driving the LED matrix

  // intitialize interrupt timer 2
  TCCR2A = 0;               // TC2 Control Register A - no ocr or pwm ; normal operation
  TCCR2B = 4;               // TC2 Control Register b - Clock Select Bit 1<<CS22 | 0<<CS21 | 0<<CS20 //prescale 1:64
  TIMSK2 = 1 << TOIE2;      // TC2 Interrupt Mask Register - TOIE2: Timer/Counter2, Overflow Interrupt Enabled
  TCNT2 = 0x06;             // The counter value (TCNT2) are initialize in 6 decimal; see calculations below:
}

void loop() {

  //delay(10000);
}

ISR(TIMER2_OVF_vect) {      // Interrupt Routine:
  TCNT2 = 0x06;
  PORTD &= 0xE3;            // clear PIN 2,3,4
  PORTD |= (n << 2) & 0x1C; // set collumn trough the mux, see:
  n++;                      // increase collumn counter
  if (n > 7) {              // count from 0 to 7
    n = 0;                  // next image. Refreshrate = 1000/8 images/sec = 125Hz (8 x ISR)
  }
}
