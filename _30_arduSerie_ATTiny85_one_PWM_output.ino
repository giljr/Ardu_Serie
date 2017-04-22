/*
  Project name:
     Fast Rainbow Circuit #arduSerie 30
     Flavour I - How to Generate a 3 PWM signal on ATtiny85
     Hex File: _30_arduSerie_ATTiny85_one_PWM_output.ino

  Revision History:
     March, 2017:

  Description:
     Getting 1 PWM from ATtiny85 and drive LED is easy.
     Read the comments and this post, please: 
     TODO[post link goes here]

  MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
  MCU:                  ATtiny85  is an 8-bit AVR Microcontroller with 8K ISP flash memory

  Connections:
      See Official Jungletronics blog: https://goo.gl/mh6TgX

  Graphics:
   https://docs.google.com/spreadsheets/d/1HCfaC4OAW71nOij7Lnwlgr5VIIuTOLhIANSV1ZYkAMM/edit?usp=sharing

  Based on: Four PWM Outputs from the ATtiny85 by David Johnson-Davies
            http://www.technoblogy.com/show?LE0#cite_note4
         And
            ATtiny PWM (updated) from I Am Nomad by Matt16060936
            http://matt16060936.blogspot.com.br/2012/04/attiny-pwm.html

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License v3 as published by
  the Free Software Foundation
*/

   /*
     ATtiny85 outputs
  */
const int Red = 1;

void setup() {

  /*
    Port B Data Direction Register (controls the mode of all pins within port B)
    DDRB is 8 bits: [unused:unused:DDB5:DDB4:DDB3:DDB2:DDB1:DDB0]
    1<<DDB0: sets bit DDB0 (data-direction, port B, pin 0), which puts PB0 (port B, pin 0) in output mode
  */
  DDRB  |= (1 << DDB0);

  /*
    Control Register A for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
    TCCR0A is 8 bits: [COM0A1:COM0A0:COM0B1:COM0B0:unused:unused:WGM01:WGM00]
    3<<COM0A0: sets bits COM0A0, COM0A1 and COM0A2, which (in Fast PWM mode) clears OC0A on compare-match, and clear OC0A at BOTTOM (inverting mode)
    3<<WGM00: sets bits WGM00 and WGM01, which (when combined with WGM02 from TCCR0B below) enables Fast PWM mode
  */
  TCCR0A = 3 << COM0A0 | 3 << WGM00;

  /*
    Control Register B for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
    TCCR0B is 8 bits: [FOC0A:FOC0B:unused:unused:WGM02:CS02:CS01:CS00]
    0<<WGM02: bit WGM02 remains clear, which (when combined with WGM00 and WGM01 from TCCR0A above) enables Fast PWM mode
    3<<CS00: sets bits CS00 and CS01 (leaving CS02 clear), which tells Timer/Counter-0 to use a prescalar 64
  */
  TCCR0B = 0 << WGM02 | 3 << CS00;

  /*
    TCCR1 is 8 bits: [CTC1:PWM1A:COM1A1:COM1A0:CS13:CS12:CS11:CS10]
    0<<PWM1A: bit PWM1A remains clear, which prevents Timer/Counter-1 from using pin OC1A (which is shared with OC0B) pin 6
    3<<COM1A0: sets bits COM1A0, COM1A1 compare match with compare register A in Timer/Counter1 - OC1A
    7<<CS10: sets bit CS10, CS11 and CS12 which tells Timer/Counter-1  to use a prescalar of 64
  */
  TCCR1 = 0 << PWM1A | 3 << COM1A0 | 7 << CS10;

  /*
    Update compare registers with red, green and blue values
    Mixing colors two by two in a well-paced fashion
    And finally a very brief pause so we can see all dacing collors
  */
  
  /*
    Cycling one-collor LED quickly
  */
}

void loop() {
  for (int i=0; i <= 254; i++) {
    OCR0A = abs(i);
    delay(1);
  }
}
