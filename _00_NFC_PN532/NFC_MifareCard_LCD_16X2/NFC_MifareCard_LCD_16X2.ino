
/*
  Project name:
     ARDUINO IN HIGH SERIE 00 - NFC ITEAD PN532 & LCD!!!
     (The 'Hello World' example for the nfc Mifare Card)
     INO File: NFC_MifareCArd_LCD_16X2.ino
   Revision History:
     20160210:
       - found on internet (gilj3)
  Description:
     let's play with the Contactless Mifarecard and  show you
     how connect NFC Module ITEAD PN532 to the Arduino and
     to simulate a security access. Enjoy good,and have fun. Bye!!!
  MCU:              Arduino - @16MHz           http://www.arduino.cc/
  NFC.Board:       NFC Module ITEAD PN532      http://wiki.iteadstudio.com/ITEAD_PN532_NFC_MODULE
  Connections:
     arduino pins 13,12,11,10, 5v, gnd ->  Itead's pins sdk, mi, mo, scl, 5v, gnd
     Arduino pins 8, 9, gnd, gnd, gnd in LCD´pins rs, e, vss, rw, k (vo in mid pot)
*/

// --- libraries ---  //

#include <PN532.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define PN532_CS 10
PN532 nfc(PN532_CS);
#define  NFC_DEMO_DEBUG 1

// --- Configurations---  //

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println("Hello!");
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
#ifdef NFC_DEMO_DEBUG
    lcd.print("Didn't find PN53x board");
#endif
    while (1); // halt
  }

  // configure board to read RFID tags and cards
  nfc.SAMConfig();
  init_display();

}

// --- Helper method---  //

void init_display() {
  // show time once in a while
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hi!! Pass Card!!");
}

// --- Infinite loop ---  //

void loop() {
  uint32_t id;
  // look for MiFare type cards
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);
  if (id != 0) {
#ifdef NFC_DEMO_DEBUG
    if (id == 3030563408) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hi Luc !!");
      lcd.setCursor(0, 1);
      lcd.print("Be Welcomed!!");
      delay(1500);
      init_display();
    }
    else if (id == 4253115926) { //(change it!) use the serial monitor
      //to find out the number of your Mifare card
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hi gilj3 !!");
      lcd.setCursor(0, 1);
      lcd.print("Be Welcomed!!");
      delay(1500);
      init_display();

    }
    else if (id == 991313862) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hi gilj3 !!");
      lcd.setCursor(0, 1);
      lcd.print("again? Be Welcomed!!");
      delay(1500);
      init_display();

    }
    else if (id) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not Autorized!!!");
      lcd.setCursor(0, 1);
      lcd.print("See Webmaster!!!");
      delay(1500);
      init_display();
    }
#endif
  }
}
