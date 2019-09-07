/*
  Project name:
     Fast Rainbow Circuit #arduSerie 30
     Flavour II - How to Generate a 2 PWM signal on ATtiny85
     Hex File: _30_arduSerie_ATTiny85_two_PWM_output.ino

  Revision History:
     March, 2017:

  Description:
     Getting 2 PWM from ATtiny85 and drive RGB LED in a round-fashion mode like raibow.
     That is no trivial task. Read the comments and this post, please: https://goo.gl/mh6TgX

  MCU:                  Arduino 1.6.4 - @16MHz       http://www.arduino.cc/
  MCU:                  ATtiny85  is an 8-bit AVR Microcontroller with 8K ISP flash memory

  Connections:
      See Official Jungletronics blog: https://goo.gl/mh6TgX

  Graphics:
   https://docs.google.com/spreadsheets/d/1HCfaC4OAW71nOij7Lnwlgr5VIIuTOLhIANSV1ZYkAMM/edit?usp=sharing

  Based on: Four PWM Outputs from the ATtiny85 by David Johnson-Davies
            http://www.technoblogy.com/show?LE0#cite_note4
         And
            ATtiny PWM (updated)from I Am Nomad by Matt16060936
            http://matt16060936.blogspot.com.br/2012/04/attiny-pwm.html

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License v3 as published by
  the Free Software Foundation
*/

   /*
     ATtiny85 outputs
  */
const int Red = 0;
const int Green = 1;

volatile uint8_t* Port[] = {&OCR0A, &OCR0B};

void setup() {

  /*
    Port B Data Direction Register (controls the mode of all pins within port B)
    DDRB is 8 bits: [unused:unused:DDB5:DDB4:DDB3:DDB2:DDB1:DDB0]
    1<<DDB1: sets bit DDB1 (data-direction, port B, pin 1), which puts PB1 (port B, pin 1) in output mode
    1<<DDB0: sets bit DDB0 (data-direction, port B, pin 0), which puts PB0 (port B, pin 0) in output mode
  */
  DDRB  |= (1 << DDB1) | (1 << DDB0);

  /*
    Control Register A for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
    TCCR0A is 8 bits: [COM0A1:COM0A0:COM0B1:COM0B0:unused:unused:WGM01:WGM00]
    3<<COM0A0: sets bits COM0A0 and COM0A1, which (in Fast PWM mode) sets OC0A on compare-match, and clear OC0A at BOTTOM (inverting mode)
    3<<COM0B0: sets bits COM0B0 and COM0B1, which (in Fast PWM mode) sets OC0B on compare-match, and clear OC0B at BOTTOM (inverting mode)
    3<<WGM00: sets bits WGM00 and WGM01, which (when combined with WGM02 from TCCR0B below) enables Fast PWM mode
  */
  TCCR0A = 3 << COM0A0 | 3 << COM0B0 | 3 << WGM00;

  /*
    Control Register B for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
    TCCR0B is 8 bits: [FOC0A:FOC0B:unused:unused:WGM02:CS02:CS01:CS00]
    0<<WGM02: bit WGM02 remains clear, which (when combined with WGM00 and WGM01 from TCCR0A above) enables Fast PWM mode
    3<<CS00: sets bits CS00 and CS01 (leaving CS02 clear), which tells Timer/Counter-0 to use a prescalar 64
  */
  TCCR0B = 0 << WGM02 | 3 << CS00;

}

  /*
    Sets colour Red=0 Green=1 to specified intensity 0 (off) to 255 (max)
  */
void SetColour (int colour, int intensity) {
  *Port[colour] = 255 - intensity;
}

  /*
    Update compare registers with red, green and blue values
    Mixing colors two by two in a well-paced fashion
    And finally a very brief pause so we can see all dacing collors
  */
void loop() {
  for (int colour = 0; colour < 2; colour++) {
    SetColour((colour + 2) % 2, 0);
    for (int i = 0; i <= 255; i++) {
      SetColour((colour + 1) % 2, i);
      SetColour(colour, 255 - i);
      delay(1);
    }
  }
}
