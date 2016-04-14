/*
 * Project name:
     03 # arduSerie - DIY 4 x 4 16-Key Numeric Keypad TEST!!    
     (The 'Hello World' example for key pad 4x4)
     http://www.dx.com/en/p/diy-4-x-4-16-key-numeric-keypad-black-149608#.Vwpw3KQrLIV
     Arduino File: keypad_testing00.ino
  * Revision History:
     20160410:
       - found on internet (gilj3)
     http://playground.arduino.cc/Code/Keypad#Download 
     https://www.youtube.com/playlist?list=PLK3PeNcUzb8TwZuXZJgREj5nDbQxRLW_a
 * Description:
      Demonstrates how to map a 4x4 keypad using the Arduino library keypad. 
      The code turns the LED on pin 13 by clicking the '*' key
      and then turn it off by clicking '#' key.  The other keys 
      reflects the key value in the serial.easy easy!! 
      Enjoy good,and have fun. Bye!!!
 * MCU:                Arduino - @16MHz   http://www.arduino.cc/
 * IC Chip:            none
 * Connections:
 * See you.tube vid: https://youtu.be/cVpLxHNI_x8
 * Based on           tutorial 'The Matrix Keypad how-to'
 * Code by Authors:   Mark Stanley, Alexander Brevig
 * Note:  The first step is to connect your keypad to the
          Arduino  using the pin numbers listed below in
          rowPins[] and colPins[]. If you want to use different
          pins then  you  can  change  the  numbers below to
          match your setup.
 * Pinout:
          1 2 3 4 5 6 7 8 9 0 (Hardware's pins)
            | | | | | | | |
            2 3 4 5 6 7 8 9   (Arduino's pins )
           |--col--|--row--|
 */

#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
  };
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 6, 7, 8, 9 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 2, 3, 4, 5 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 13

void setup()
{
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600);
}

void loop()
{
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        digitalWrite(ledpin, LOW);
        break;
      case '#':
        digitalWrite(ledpin, HIGH);
        break;
      default:
        Serial.println(key);
    }
  }
}
