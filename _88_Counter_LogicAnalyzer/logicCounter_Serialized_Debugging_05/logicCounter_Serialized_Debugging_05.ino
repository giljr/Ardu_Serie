/*
   Binary Counter.

   With 8xLEDs we can display binary numbers from 0 to 255.

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
int waitTime=100;

/*
 * Set each pin to be used as output, and default to zero.
 */
void setup(void)
{
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  count = 0;
}

/*
 *  Constantly increase, display, and delay.
 */
void loop(void)
{
  dispBinary(count++);
  delay(waitTime);
}


/*
 *  Show a single number.
 */
void dispBinary(byte n)
{
  for (byte i = 0; i < 4; i++) {
      Serial.print(i);
      Serial.print(",");
      Serial.print("n=");
      Serial.print(n);
      Serial.print(",");
    digitalWrite(ledPins[i], n & 1);
      Serial.print(n);
      Serial.print(",");
    n /= 2;
      Serial.print(n);
      Serial.println(",");
   } 
      Serial.println("");
}
