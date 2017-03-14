
/*
   Project name: #arduSerie-25
     Studing Bitwise Operations
     Flavour III - Simple Game of PONG Frame 3
     Hex File: _25_bitwise_pong_game_01.ino
   Revision History:
     20170110:
       - from https://www.arduino.cc/en/Reference/BitwiseAnd
       and https://youtu.be/evmbh2ntqio?list=PLZ8dBTV2_5HSyOXhJ77d-iyt5Z_v_1DPM
   Description:
           * In this second code we will add the paddle bats and the ball running from
           * the right to the left in a straight line. The idea is to gradually add
           * the implementations to patiently understand the code at a advanced stage.
           * The paddle right & left always follows the ball as we are
           * in the automatic mode so far. To form the full screen we interact eight times
           * in the image object (imageMem with 8 bytes), which holds the positions
           * of the three objects all at once: two rackets and a ball dinamically.
           * See the images to understand the concept.
           
   MCU:                  Arduino 1.8.2 - @16MHz       http://www.arduino.cc/ 
   Connections:
      See Official youtube channel vids:

   Based on:
        Led Matrix
                    From Small Electronic Thingies for Doing All Kinds of Fun Stuff
                    http://wiki.edwindertien.nl/doku.php?id=modules:ledmatrix   
        Bitwise AND (&), Bitwise OR (|), Bitwise XOR (^)
                    from https://www.arduino.cc/en/Reference/BitwiseAnd
               
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

byte imageMem[8] = {                     // display buffer
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
float speed_x = 1;                      // ball variables: speed & positions(pos_x/pos_y)
float speed_y = 0.5;                    // transversal speed (y axis)
float pos_x = 0;                        // ball position in axis 
float pos_y = 4;                        // Range for the complete paddle bat design from 1 up to 6
float paddle_l, paddle_r;               // paddle positions
int n;                                  // display row counter
void setup() {
  for (int z = 2; z < 14; z++) {
    pinMode(z, OUTPUT);                 // all pins output for driving the LED matrix
  }
                                        
  TCCR2A = 0;                           // no ocr or pwm; intitialize interrupt timer 2
  TCCR2B = 4;                           // 1<<CS22 | 0<<CS21 | 0<<CS20 -- for clock div 64
  TIMSK2 = 1 << TOIE2;                  // enable timer 1 interrupt
  TCNT2 = 0x06;                         // 1 kHz - see datasheet (https://goo.gl/k7tXKa)
}
void loop() {
if (speed_x == -1) paddle_l = pos_y;  // autoplay only
  else paddle_r = pos_y;                // The paddle bat always chases the ball along the  y axis
                                        // and never misses!!!
  if ((pos_x > 6) && (speed_x == 1) && (pos_y < (paddle_r + 1)) && (pos_y > (paddle_r - 1))) {
    
    speed_x = -1;                       // reverse direction when hitting the  right bat
  }
  if ((pos_x < 1) && (speed_x == -1) && (pos_y < (paddle_l + 1)) && (pos_y > (paddle_l - 1))) {
    
    speed_x = 1;                        // reverse direction when hitting the left bat
  }
  pos_x += speed_x;
  
  clearScreen();
  drawBall((int)pos_x, (int)pos_y);
  drawBats((int)paddle_l, (int)paddle_r);
  delay(200);                            // how fast are the ball...You can change
  //delay(50);                           //to fifty and see full speed ball...
}
void drawBats(int l, int r) {           // draw bats by number 3d = 0000 0011 - draw right paddle bat.
  imageMem[7] = (3 << l) >> 1;          // shift 0011 left by 'int l'[0..7] and then shift right result by 1 
                                        // see: suppose l = 7 -> (0000 0011 << 7) >> 1 results 0110 0000
  imageMem[0] = (3 << r) >> 1;          // same as above, except by r [0..7] value - draw right paddle bat.
                                        // draw bats in imageMem like this  [][][][*][*][][][] see diagram
}
void drawBall(int x, int y) {           // Draw a bit of the ball in any of the positions that appear 
                                        // asterisk in the imageMem [][*][*][*][*][*][*][]
     imageMem[7 - x] = 1 << y;          // draw the ball is very easy, just get its positions inside imageMem
                                        // And dumps the six bytes at each delay ms from loop()
}                                   
void clearScreen() {
  for (int z = 0; z < 8; z++) {         // clear all bytes in imageMem, this routine is called to 
                                        // each loop, thus zeroing 8 bytes to new scene stage. see simulation images
    imageMem[z] = 0x00;
  }
}
ISR(TIMER2_OVF_vect) {                  // see complete anotation on code I 
  TCNT2 = 0x06;                         // reload timer at 1kHz
  PORTD &= 0xE3;                        // clear PIN 2,3,4
  PORTD |= (n << 2) & 0x1C;             // set collumn trough the mux
  PORTD &= 0x1F;                        // clear PIN 5,6,7
  PORTD |= (imageMem[n] << 5) & 0xE0;   // set row data on 5,6,7
  PORTB &= 0xE0;                        // clear PIN 8,9,10,11,12
  PORTB |= (imageMem[n] >> 3) & 0x1F;   // set row data on 8-12
  n++;                                  // increase collumn counter
  if (n > 7) {
    n = 0;                              // next image. Refreshrate = 1000/8 images/sec
  }
}
