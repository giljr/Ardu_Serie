/*
  Project name:
     ABC Project: Arduino, Bleetooth and Compass V1.0 - 24 #arduSerie
     (ABC project - Arduino, Bluetooth and Compass Project!!!)
     Flavour I - How Bluetooth Work
     Hex File: _24_arduSerie_Arduino_Bluetooth.ino
   Revision History:
     20161231:
       - from How To Make a Compass using Arduino and Processing IDE
         by Dejan Nedelkovski
       http://howtomechatronics.com/projects/how-to-make-a-compass-using-arduino-and-processing-ide/

   Description:
     We will implement a compass connected to the arduino via bluetooth. We used the HC-06 bluetooth dongle, 
     GY-85 board, which sensor used are only HMC5883L out of three.
     
   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   HC-06:                Wireless RF Transceiver Bluetooth HC-06 Slave Module RS232/TTL for Arduino http://www.ebay.com/bhp/hc-06
   GY-85:                https://github.com/madc/GY-85
  
   Connections:
      See Official youtube channel vids:
      GY-85:         BT:
      SDA-A4         RXD-1K2+2K2 RESISTOR VOLTAGE DIVISOR+ARDUINO'S PIN 1 TX
      SDL-A5         TXD-PIN 0 ARDUINO's RX
      3V3-3V3        VCC-5V 
      GND-GND        GND-GND  
    
   Based on:   How To Make a Compass using Arduino and Processing IDE by Dejan Nedelkovski
               and 
               TECHBITAR (HAZIM BITAR) EMAIL: techbitar.gmaiL.com
                      
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
/*   Arduino Compass 
 *      
 *  by Dejan Nedelkovski, 
 *  www.HowToMechatronics.com
 *  
 */

//////////////////////////////////////////////////////////////////////////////////
// REMIXED BY: TECHBITAR (HAZIM BITAR)
// LICENSE: PUBLIC DOMAIN
// DATE: MAY 2, 2012
// CONTACT: techbitar at gmail dot com

int counter = 0;

void setup() {
  Serial.begin(9600);
  delay(50);
}

void loop() {
  counter++;
  Serial.print("Arduino counter: ");
  Serial.println(counter);
  delay(500); // wait half a sec
}
