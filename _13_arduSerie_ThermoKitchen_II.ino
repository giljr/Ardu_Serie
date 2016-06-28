/*
  Project name:
     Arduino 1-Wire Thermocouple in the kitchen- 13 # arduSerie
     Flavour I
     Hex File: _13_arduSerie_ThermoKitchen_II.ino
   Revision History:
      20160627:
      Description:
      This serie makes several experiments with thermocouple in the home kitchen.
      The purpose is to record the temperature over time. We will experiment with 
      cheese bread, sandwich cheese, gelatin and more. Here we will make the 
      thermocouple works without any components. Get on!!! Check it over !!!

  MCU:              Arduino v.1.6.7 - @16MHz  http://www.arduino.cc/
  IC Chip:          MAX31850K 
      https://www.adafruit.com/products/1727
  Library: Adafruit_MAX31855_lib 
      https://github.com/adafruit/Adafruit-MAX31855-library
  Connections:
      See you.tube vid:
  Based on https://learn.adafruit.com/thermocouple/using-a-thermocouple
      Tutorial - Adafruit Learning System
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
/***************************************************
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K
  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269
  These displays use SPI to communicate, 3 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_MAX31855.h"
#include <LiquidCrystal.h>

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5
#define LCD_LIGHT_PIN A2  
// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
  

#define BACKLIGHT_PIN     13
int but = 2;                                         // button to control when to display in LCD data

//LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7);   // 0x27 is the I2C bus address for an unmodified backpack
LiquidCrystal lcd(A0, A1, 10, 9, 8, 7, 6);

//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address


// Creat a set of new characters
const uint8_t charBitmap[][8] = {
  { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
  { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
  { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
  { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
  { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
  { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
  { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
  { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }

};

void setup()
{
  // Set the lcd display backlight anode pin as an output.
  pinMode(LCD_LIGHT_PIN, OUTPUT);
  digitalWrite(LCD_LIGHT_PIN, LOW);
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));

  // Switch on the backlight
  //lcd.setBacklightPin(3, NEGATIVE);
  //lcd.setBacklight(LOW);
  lcd.begin(16, 2);              // initialize the lcd

  for ( int i = 0; i < charBitmapSize; i++ )
  {
    lcd.createChar ( i, (uint8_t *)charBitmap[i] );
  }

  lcd.home ();                   // go home
  lcd.print("Firmware v 0.1 ");
  //lcd.setCursor ( 0, 1 );        // go to the next line
  //lcd.print (" Date - jun 2016   ");
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  //lcd.begin(16, 2);

  //lcd.clear();
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print("MAX31855 setup");
  // wait for MAX chip to stabilize
  delay(500);

  delay ( 4000 );
}

void loop()
{
   // Switch on the backlight

  
  lcd.home ();
  // Do a little animation by writing to the same location
  for ( int i = 0; i < 2; i++ )
  {
    for ( int j = 0; j < 16; j++ )
    {
      lcd.print (char(random(7)));
    }
    lcd.setCursor ( 0, 1 );
  }
  delay (200);


  if (digitalRead(but) == HIGH)                      // if you push the button to show the maximum rpm
  { // basic readout test, just print the current temp
    // Switch on the backlight
    //lcd.setBacklightPin(3, POSITIVE);
    //lcd.setBacklight(LOW);
    digitalWrite(LCD_LIGHT_PIN, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Int Temp = ");
    lcd.println(thermocouple.readInternal());
    Serial.print("Int. Temp = ");
    Serial.println(thermocouple.readInternal());

    double c = thermocouple.readCelsius();
    lcd.setCursor(0, 1);
    if (isnan(c))
    {
      lcd.print("T/C Problem");
    }
    else
    {
      lcd.print("C = ");
      lcd.print(c);
      lcd.print("  ");
      Serial.print("Thermocouple Temp = *");
      Serial.println(c);
    }

    delay(1000);
  }
    //lcd.setBacklightPin(3, NEGATIVE);
    //lcd.setBacklight(HIGH);
    digitalWrite(LCD_LIGHT_PIN, LOW);
}
