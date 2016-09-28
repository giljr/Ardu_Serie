/*
  Project name:
     16 #arduSerie - Arduino Headless Setup  & ESP 8266 
     (WIFI cheap chip!)
     Flavour IV - 
     Hex File: _16_arduSerie_sketch_04_Ir_Test.ino
   Revision History:
     20160927:
       - board found on Arduino IDE Exemples
     (connect everything on ESP8266 to access examples)
   Description:
     Here we use the two platforms independently. 
     Aarduino reading the SHARP GPY0A41SK0F sensor and 
     go ESP8266 serving web page to flash an LED connected to gpio2.
     http://server_ip/gpio/0 will set the GPIO2 low,
     http://server_ip/gpio/1 will set the GPIO2 high
     SEE: _16_arduSerie_sketch_03.WiFiWebServer.ino
     
   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   IC Chip:              ESP8266 - SDK                 https://github.com/pfalcon/esp-open-sdk
  
   Connections:
      See Officil you.tube channel vids: http://goo.gl/unfHZA

   Based on:   Renato Aloi's SerieLink - Curso Arduino Express Aula 14 - ESP8266 Part I and II (seriallink.com.br)
   https://youtu.be/c4u_JbbvzxE
   Code by esp-community HelloServer
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation

*/

int reading;

int myir = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(myir, OUTPUT);
}

void loop()
{

  reading = analogRead(myir);
  delay(100);
  Serial.println(reading);
}
