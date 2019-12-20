/* Project: Ardu_Series # 88
     Binary Counter - Logic Analyzer - Building an LED Binary Counter & Improving The Code

     INO file: _88_logicCounter_Extensible_4bits_03.ino

    Binary Counter.

    Description: With 4xLEDs we can display binary numbers from 0 to 15;
                 This code consomes just 2326 bytes (3% of program storage space)
                 and Global variables use 26 bytes (1%) of dynamic memory:)
                 and beyond this, it has Extensibility!
                 Extensibility is a software engineering and systems design principle
                 that provides for future growth. Extensibility is a measure of the
                 ability to extend a system and the level of effort required to implement the extension.

    Wiring involves:

   Output PIN XX -> LED -> 220k resister -> Ground

   Repeat that for each of the eight LEDs.

    In my case I wired them on pins 1,2,3,4,5,6,7,8.

*/

// The pins to which we've wired each of the LEDs
int ledPins[] = {1, 2, 3, 4};

// The number we're going to display.
byte count;

// The delay between each count
int waitTime = 500;

/*
   Set each pin to be used as output, and default to zero.
*/
void setup(void)
{
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  count = 0;
}

/*
    Constantly increase, display, and delay.
*/
void loop(void)
{
  dispBinary(count++);
  delay(waitTime);
}

/*
    Show a single number.
*/
void dispBinary(byte n)
{
  for (byte i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], n & 1);
    n /= 2;
  }
}
