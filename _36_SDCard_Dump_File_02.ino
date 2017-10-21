/*
   Project name: JayThree Time_Lapse CAM Project #arduSerie 36
     A very cool project that captures images for a predetermined time. 
     Serves as a time lapse camera. It is widely used in biology, 
     for the study of human or animal behavior. Have a good time!!!
     
     Flavour II - SDCard Dumping File 
     INO File: _36_SDCard_Dump_File_02.ino
   
   Revision History:
     Oct, 2017
       - v 1.0 - Medium webpage:
   Description:
           Testing openning file: log.csv (it must be on the disk - run Flavor I)
           This example shows how to read once a file from the SD card using the
           SD library and send it over the serial port. It does not use loop()!
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
/*
  SD card file dump
  Testing openning file: datalog.txt
 
 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
  created  22 December 2010
 by Limor Fried
 modified 02 Jun 2014
 by Gil Jr
  This example code is in the public domain.
  */

#include <SPI.h>
#include <SD.h>

                                // On the Ethernet Shield, CS is pin 4. Note that even if it's not
                                // used as the CS pin, the hardware CS pin (10 on most Arduino boards,
                                // 53 on the Mega) must be left as an output or the SD library
                                // functions will not work.
                                // On the Ethernet Shield, CS is pin 4. Note that even if it's not
                                // used as the CS pin, the hardware CS pin (10 on most Arduino boards,
                                // 53 on the Mega) must be left as an output or the SD library
                                // functions will not work.
//Set by default for the SD card library for SparkFun Pro Micro 5v
//MOSI = Pin 11
//MISO = Pin 16
//SCLK = PIN 15
//Set for Arduino UNO
//MOSI = Pin 11
//MISO = Pin 12
//SCLK = PIN 13
//We always need to set the CS Pin
const int CS_PIN = 10;
const int SD_POW_PIN  = 8;

void setup()
{
                                // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //Serial1.begin(9600);        // For SparkFun Arduino Pro Micro - put 1 on end of Serial[1]
   while (!Serial) {
    ;                           // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
   
  pinMode(CS_PIN,   OUTPUT);         // CS pin is an output
  pinMode(SD_POW_PIN, OUTPUT);       // Supply 5v to SDCard
  digitalWrite(SD_POW_PIN, HIGH);    // Card will draw power from pin 8, so set it high
  
                                // see if the card is present and can be initialized:
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card failed, or not present");
                                // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
                                // open the file. note that only one file can be open at a time,
                                // so you have to close this one before opening another.
  File dataFile = SD.open("log.csv");

                                // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }  
                                // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening log.csv");
  } 
}

void loop()
{
}

