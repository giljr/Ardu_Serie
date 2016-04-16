/*
 * Project name:
     02 # ardu_serie - 8 Digit 7 Segment Display   
     (The 'Hello World' example for 8-digit w/ IC 74HC595)
     Arduino File: 8Digit_00.ino
  * Revision History:
     20160415:
  * Description:
      This 7 segmet, 8 digit LED Display uses 75HC595 to be driven
      by an MCU using only 3 IO pins. We do a count following position
      one to position eight and then came back to score. Very simple. 
      More conceptual than functional. Welcome!!!
 * MCU:                Arduino - @16MHz   http://www.arduino.cc/
 * IC Chip:            74HC595 - 8-bit serial-in/parallel-out shift register 
 * Connections:        DIO=9; SKC=10; RCK=11(HARD/ARD)
 * See you.tube vid: https://youtu.be/CKetrf3Mv9s
 * Based on  Chapter 7 Shift Registers/ Book by Jeremy Blum 
 *             http://www.exploringarduino.com/content/ch7/      
 */


#define LATCH  10  //Serial Output to Shift Register
#define CLOCK  11 //Shift Register Clock Pin
#define DATA   9  //pin 10 connect to DIO
#define BUZZ    3  //pin 3  connect to Buzzer
#define BUTTON  5  //pin 5 connect to Button

// array to activate particular digit on the 8x7segment module
// it is the common anode of 7 segment
byte POS[8] = { 0b10000000,  //digit 1 from right
                0b01000000,  //digit 2 from right
                0b00100000,  //digit 3 from right
                0b00010000,  //digit 4 from right
                0b00001000,  //digit 5 from right
                0b00000100,  //digit 6 from right
                0b00000010,  //digit 7 from right
                0b00000001   //digit 8 from right
              };

//array for decimal number, it is the cathode.
//logic low will activate the particular segment
byte NUM[13] = {
  //0b11000000,  // 0
  0b11111001,  // 1
  0b10100100,  // 2
  0b10110000,  // 3
  0b10011001,  // 4
  0b10010010,  // 5
  0b10000010,  // 6
  0b11111000,  // 7
  0b10000000,  // 8
  //0b10010000,  // 9
  //0b01111111,  //dot
  //0b11111111,  //blank
  //0b10111111  //bomb disarmed
};
//fucntion to send the serial data out to two 74HC595 serial to parallel shift register and activate the 7 segment.
void display8x7segment(byte datapin, byte clockpin, byte latchpin, byte POS, byte NUM)
{
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, POS); // Particular digit position
  shiftOut(DATA, CLOCK, MSBFIRST, NUM); // array for decimal number
  digitalWrite(LATCH, HIGH);
}

void setup()
{
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  //pinMode(BUZZ, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(LATCH, HIGH); //Wake Up Display
  digitalWrite(BUZZ, LOW);  //Buzzer Off
  digitalWrite(BUTTON, HIGH); // Button Low
  delay(1000);  //delay for 1 second

}

void loop()
{
  for (byte e = 0; e < 7 ; e ++) {
    display8x7segment(DATA, CLOCK, LATCH, POS[e], NUM[e]);
    delay(300);
  }
  for (byte d = 7; d > 0 ; d --) {
    display8x7segment(DATA, CLOCK, LATCH, POS[d], NUM[d]);
    delay(300);
  }

}
