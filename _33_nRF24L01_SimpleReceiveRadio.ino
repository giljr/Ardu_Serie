//*
   Project name: nRF24L01 Radios Hello World! #arduSerie-33
     Studing nRF24L01 radios!
     Flavour II - Receiver Radio
     Hex File: _33_nRF24L01_SimpleReceiveRadio.ino
   Revision History:
     Jun, 2017
       - Medium webpage:
   Description:
          This is a simple code to configure a radio transmitter and other receiver 
          in the simplest possible way!
   MCU:                  Arduino 1.8.2 - @16MHz       http://www.arduino.cc/
   Radio:                nRF24L01  - Unknown version  
   Connections:
      See Official youtube channel vids:
   Based on:  Connecting and programming nRF24L01 with Arduino and other boards
               from http://starter-kit.nettigo.eu/2014/connecting-and-programming-nrf24l01-with-arduino-and-other-boards/
            
   Datasheet Atmega328P  8-bit AVR Microcontrollers:
   http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";          // you can modify this address 
                                         // serves to mark each transmission line

void setup()
{
  while (!Serial);
  Serial.begin(9600);                    // initializa the serial interface
  
  radio.begin();                         // initializa the modem
  radio.openReadingPipe(0, rxAddr);      // open a stream for reading the data pipe
  
  radio.startListening();                // start the process of listening
}

void loop()
{
  if (radio.available())                 // will I have date to receive in buffer?
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));     // if yes, read it!
    
    Serial.println(text);                //send to arduino serial
  }
}
