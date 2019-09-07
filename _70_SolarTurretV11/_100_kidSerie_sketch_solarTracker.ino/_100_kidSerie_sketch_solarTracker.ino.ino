/*
  Project name: ArduSerie #69
     Solar Turret V 1.1 - Improving Design 4 Learning More 
     Imported from:
     100 #kidSerie - J3 Solar Tracker Turret V1.0
     (Awesome project!!!)
     Flavour I -
     Hex File: _100_kidSerie_sketch_solarTracker.ino
   Revision History:
     20161219:
       - from github adafruit/Adafruit_MQTT_Library
  https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
  https://learn.adafruit.com/mqtt-adafruit-io-and-you/intro-to-adafruit-mqtt

   Description:
     This is a Awesome Solar Tracker Turret v1.0, based on Arduino Sun Tracker Turret © CC0.
     https://create.arduino.cc/projecthub/robotgeek-projects-team/arduino-sun-tracker-turret-06cba9?utm_source=newsletter&utm_campaign=Nov15&utm_medium=banner&utm_content=solartr

   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   Servos:               9 g                           http://www.dx.com/p/2-9g-servos-bracket-sensor-mount-pan-tilt-kit-for-gyro-translucent-blue-214081#.WFhA2PkrLDc
   LDR                   4x                            http://www.instructables.com/id/How-to-Use-a-Light-Dependent-Resistor-LDR/

   Connections:
      See Official youtube channel vids:
   Based on:   Arduino Sun Tracker Turret © CC0

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/


#include <Servo.h> // include Servo library 

Servo horizontal; // horizontal servo
int servoh = 90;     // stand horizontal servo

Servo vertical;   // vertical servo
int servov = 90;     // stand vertical servo

// LDR pin connections
//  name  = analogpin;
int ldrlt = 0; //LDR top left
int ldrrt = 1; //LDR top rigt
int ldrld = 2; //LDR down left
int ldrrd = 3; //ldr down rigt

void setup()
{
  Serial.begin(9600);
  // servo connections
  // name.attach(pin);
  horizontal.attach(9);
  vertical.attach(10);
}

void loop()
{
  int lt = analogRead(ldrlt); // top left
  int rt = analogRead(ldrrt); // top right
  int ld = analogRead(ldrld); // down left
  int rd = analogRead(ldrrd); // down rigt

  int dtime = analogRead(4) / 20; // read potentiometers
  int tol = analogRead(5) / 4;

  int avt = (lt + rt) / 2; // average value top
  int avd = (ld + rd) / 2; // average value down
  int avl = (lt + ld) / 2; // average value left
  int avr = (rt + rd) / 2; // average value right

  int dvert = avt - avd; // check the diffirence of up and down
  int dhoriz = avl - avr;// check the diffirence og left and rigt

  if (-1 * tol > dvert || dvert > tol) // check if the diffirence is in the tolerance else change vertical angle
  {
    if (avt > avd)
    {
      servov = ++servov;
      if (servov > 180)
      {
        servov = 180;
      }
    }
    else if (avt < avd)
    {
      servov = --servov;
      if (servov < 0)
      {
        servov = 0;
      }
    }
    vertical.write(servov);
  }

  if (-1 * tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
    if (avl > avr)
    {
      servoh = --servoh;
      if (servoh < 0)
      {
        servoh = 0;
      }
    }
    else if (avl < avr)
    {
      servoh = ++servoh;
      if (servoh > 180)
      {
        servoh = 180;
      }
    }
    else if (avl = avr)
    {
      // nothing
    }
    horizontal.write(servoh);
  }
  delay(dtime);
}
