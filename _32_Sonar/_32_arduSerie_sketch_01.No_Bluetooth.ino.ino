/*
  Project name: 
     Sonar Judge The Distance Circuit - Circuito ‘Golpe de Vista’ #arduSerie — 32
     Flavour I - Without Bluetooth
     Hex File: _32_arduSerie_sketch_01.No_Bluetooth.ino
   Revision History:
     14/05/2017:
Description:
        How about that, sound and vibrate your cell? There you are! 
        Here is a Golpe de Vista circuit for building garages that are getting tighter and tighter.
        
    MCU:                     Arduino 1.8.2 - @16MHz       http://www.arduino.cc/
IC Chip:                     Bluetooth Datasheet - HC-06   https://goo.gl/s9ZcNC
Connections:
      See Official youtube channel vids: https://goo.gl/BhLG8C
      
Based on:  see bellow
  
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License v3 as published by the Free Software Foundation
   
*/
/*
  HC-SR04 Ping distance sensor]
  VCC to arduino 5v GND to arduino GND
  Echo to Arduino pin 13 Trig to Arduino pin 12
  Red POS to Arduino pin 11
  Green POS to Arduino pin 10
  560 ohm resistor to both LED NEG and GRD power rail
  More info at: http://goo.gl/kJ8Gl
  Original code improvements to the Ping sketch sourced from Trollmaker.com
  Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin 13
#define echoPin 12
#define stop_LED 11
#define go_LED 10

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(stop_LED, OUTPUT);
  pinMode(go_LED, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);     // Added this line
  delayMicroseconds(2);           // Added this line
  digitalWrite(trigPin, HIGH);
                                  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10);          // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration ) / 58.2;
  if (distance < 5) {             // This is where the LED On/Off happens
    digitalWrite(stop_LED, HIGH); // When the Red condition is met, the Green LED should turn off
    digitalWrite(go_LED, LOW);
  }
  else {
    digitalWrite(stop_LED, LOW);
    digitalWrite(go_LED, HIGH);
  }
  delay(500);
}
