/*  Project: Ardu_Serie # 83
             74HC4067 — DeMux For Arduino
             A multiplexer of this sort really just acts as a 16 to one 1 switch

     INO file: _83_74HC4067_multiplexer_demo_01.ino

     date: 8/30/19

    code by: https://tronixstuff.com
    hardware by: Ghttps://pt.aliexpress.com/wholesale?catId=0&initiative_id=SB_20190830133819&isPremium=y&SearchText=74HC4067+16-Channel+Analog+&switch_new_app=y
        software: Arduino IDE 1.8.9

    Description: Instead of upgrading your microcontroller, with this board
                 you can to expand the I/O capabilities of your chosen microcontroller

    Visit: https://medium.com/jungletronics

    Tutorial: https://medium.com/jungletronics/74hc4067-demux-for-arduino-7f7892975edf

    License: CC-SA 3.0, feel free to use this code however you'd like.
    Please improve upon it! Let me know how you've made it better.
*/
// 74HC4067 demultiplexer demonstration (1 to 16)

// control pins output table in array form
// see truth table on page 2 of TI 74HC4067 data sheet
// connect 74HC4067 S0~S3 to Arduino D7~D4 respectively
// 5V to 74HC4067 pin 1 to power the LEDs 🙂
byte controlPins[] = {B00000000,
                      B10000000,
                      B01000000,
                      B11000000,
                      B00100000,
                      B10100000,
                      B01100000,
                      B11100000,
                      B00010000,
                      B10010000,
                      B01010000,
                      B11010000,
                      B00110000,
                      B10110000,
                      B01110000,
                      B11110000
                     };

void setup()
{
  DDRD = B11111111;               // set PORTD (digital 7~0) to outputs
}

void setPin(int outputPin)
// function to select pin on 74HC4067
{
  PORTD = controlPins[outputPin];
}

void loop()
{
  for (int i = 0; i < 16; i++)
  {
    setPin(i);
    delay(250);
  }
}
