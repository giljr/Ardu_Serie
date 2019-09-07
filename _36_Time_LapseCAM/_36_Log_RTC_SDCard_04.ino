/*
   Project name: JayThree Time_Lapse CAM Project #arduSerie 36
     A very cool project that captures images for a predetermined time. 
     Serves as a time lapse camera. It is widely used in biology, 
     for the study of human or animal behavior. Have a good time!!!
     
     Flavour IV - Logging RTC data to SDCard  
     INO File: _36_Log_RTC_SDCard_04.ino
   
   Revision History:
     Oct, 2017
       - v 1.0 - Medium webpage:
   Description:
            This code storage RTC data in the SDCard at the frequency established in the variable 'refresh_rate';
            The format: <timestamp, message> ex: <2017/10/12,21:4:59,Hello There!>
            Want another Refresh Rate? PLEASE WRITE XXXX Refresh Rate in the first line of the LOG.CSV 
            (ex: 5000) file so Reading Command File will parse this integer and make the Card Ready to Logging. 
            If the SDCard is virgem it will create a LOG.CSV and refresh at 5 ms the RTC timestamp. 
            Fast Format the SDCard Disk after save file for your CPU - you'll get better result!
            **********Be careful when inserting the disk into the module - ensure that the card lock switch does 
            not close the disk for read or write. I put a tape to hold the key ;)**********
   MCU:                  Arduino Pro Micro - Atmega328 16MHz 5V @16MHz - https://www.sparkfun.com/products/12640
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   SDCard:               SD Card Module Slot Socket Reader LC Studio 3.3V/5V - https://goo.gl/gCUnEU
   DS1307 RTC:           DS1307 Real Time Clock breakout board - https://www.adafruit.com/product/264
   Serial Camera:        Grove - Serial Camera Kit - https://www.seeedstudio.com/Grove-Serial-Camera-Kit-p-1608.html?cPath=25_33                            
   Connections:          See Official youtube channel vids: https://goo.gl/unfHZA
   Based on:             http://jungletronics.blogspot.com.br/2014/06/blog-post.html
   Credit:               JR TimeLapse Cam Project!                  
   Cam Datasheet (pdf):  https://goo.gl/oGBQbx  (CJ-OV528 Protocol（通信协议） (2))
   Terms of use:  
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
     THE SOFTWARE.    
  //original notes:
/*
Exploring Arduino - Code Listing 13-3: SD Reading and Writing with an RTC
http://www.exploringarduino.com/content/ch13

Copyright 2013 Jeremy Blum ( http://www.jeremyblum.com )
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License v3 as published by
the Free Software Foundation.
*/

                                      // SD read and write with RTC
#include <SPI.h>
#include <SD.h>                       // For talking to SD Card
#include <Wire.h>                     // For RTC
#include "RTClib.h"                   // For RTC
                              
const int CS_PIN      =10;            // Define pins
const int SD_POW_PIN  =8;             // SD card is on standard SPI pins
const int RTC_POW_PIN =A3;            // RTC is on Standard I2C Pins
const int RTC_GND_PIN =A2;
                                      
int refresh_rate = 5000;              // Default rate of 5 seconds
                                      
RTC_DS1307 RTC;                       // Define an RTC object

                                      // Initialize strings
String year, month, day, hour, minute, second, time, date;

void setup()
{
  Serial.begin(9600);
  //Serial1.begin(9600);              // For SparkFun Arduino Pro Micro - put 1 on end of Serial[1]
  Serial.println(F("Initializing Card"));
 
                                      // CS pin, and pwr/gnd pins are outputs
  pinMode(CS_PIN,   OUTPUT);
  pinMode(SD_POW_PIN, OUTPUT);
  pinMode(RTC_POW_PIN, OUTPUT);
  pinMode(RTC_GND_PIN, OUTPUT);
 
                                      // Setup power and ground pins for both modules 
  digitalWrite(SD_POW_PIN, HIGH);
  digitalWrite(RTC_POW_PIN, HIGH);
  digitalWrite(RTC_GND_PIN, LOW);
 
                                      // Initiate the I2C bus and the RTC library
  Wire.begin();
  RTC.begin();
 
                                      // If RTC is not running, set it to the computer's compile time
  if (! RTC.isrunning())
  {
    Serial.println(F("RTC is NOT running!"));
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
                                      // Initialize SD card
  if (!SD.begin(CS_PIN))
  {
    Serial.println(F("Card Failure"));
    return;
  }
  Serial.println(F("Card Ready"));
 
                                      // Read the configuration information (LOG.CSV)
  File commandFile = SD.open("LOG.CSV");
  if (commandFile)
  {
    Serial.println(F("Reading Command File"));
  
    while(commandFile.available())
    {
      refresh_rate = commandFile.parseInt();
    }
    Serial.print(F("Refresh Rate = "));
    Serial.print(refresh_rate);
    Serial.println(F("ms"));
    commandFile.close();
  }
  else
  {
    Serial.println(F("Could not read command file."));
    return;
  }
 
                                      // Write column headers
  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(F("\nNew Log Started!"));
    dataFile.println(F("Date,Time,Phrase"));
    dataFile.close();                 // Data isn't actually written until we close the connection!
  
                                      // Print same thing to the screen for debugging
    Serial.println(F("\nNew Log Started!"));
    Serial.println(F("Date,Time,Phrase"));
  }
  else
  {
    Serial.println(F("Couldn't open log file"));
  }
 
}

void loop()
{
                                      // Get the current date and time info and store in strings
  DateTime datetime = RTC.now();
  year  = String(datetime.year(),  DEC);
  month = String(datetime.month(), DEC);
  day  = String(datetime.day(),  DEC);
  hour  = String(datetime.hour(),  DEC);
  minute = String(datetime.minute(), DEC);
  second = String(datetime.second(), DEC);
 
                                      // Concatenate the strings into date and time
  date = year + "/" + month + "/" + day;
  time = hour + ":" + minute + ":" + second;
 
  String dataString = "Hello There!";

// Example for output options: 
// Disk are empty                         // Disk have a LOG.CSV file
//                                        // 1ª line -> LOG.CSV=10000
// Output File:                           // Output File:
//                                        //
// [ Card Ready                       ]   // [ Initializing Card                ]
// [ Could not read command file.     ]   // [ 20017/10/13,17:33:56,Hello There!]
// [                                  ]   // [ Initializing Card                ]
// [ 2017/10/13,17:42:23,Hello There! ]   // [ Card Ready                       ]
// [ 2017/10/13,17:42:28,Hello There! ]   // [ Reading Command File             ]
// [ 2017/10/13,17:42:33,Hello There! ]   // [ Refresh Rate = 10000ms           ]
// [ 2017/10/13,17:42:38,Hello There! ]   // [                                  ]
//                                        // [                                  ] 
//                                        // [ New Log Started!                 ]
//                                        // [ Date,Time,Phrase                 ]
//                                        // [ 2017/10/13,17:33:59,Hello There! ]
//                                        // [ 2017/10/13,17:34:9,Hello There!  ]
//                                        // [ 2017/10/13,17:34:19,Hello There! ]

  File dataFile = SD.open("log.csv", FILE_WRITE);
  if (dataFile)
  {
    dataFile.print(date);
    dataFile.print(F(","));
    dataFile.print(time);
    dataFile.print(F(","));
    dataFile.println(dataString);
    dataFile.close();                 // Data isn't actually written until we close the connection!
  
                                      // Print same thing to the screen for debugging
    Serial.print(date);
    Serial.print(F(","));
    Serial.print(time);
    Serial.print(F(","));
    Serial.println(dataString);
  }
  else
  {
    Serial.println(F("Couldn't open log file"));
  }
  delay(refresh_rate);
}

