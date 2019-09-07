/*
   Project name:
     05 # arduSerie - DD_ARD_YU v1.0 - Digital Dimmer ARDuino for YoU
     (The 'Hello World' example for TRIAC)
     flavour III
     Arduino File: ac_light_dimmer_adapt_03.ino
   Revision History:
     20160430:
   Description:
     Using IR Sharp Sensor
     We will discuss the operation of TRIAC, MOC and 4n25 in its essence. We bring several graphics
     for better understanding of the basic concepts. Let's master the alternating current.
     Be careful, be conscious of the dangers of handling high-voltage currents.
     You can kill yourself !!! Read license, please !!!

   MCU:                Arduino - @16MHz   http://www.arduino.cc/
   IC Chip:            MOC3021, TIC246B, BRIDGE RET W10M, 4N25
   IR Sensor:          Infrared Proximity Sensor - Sharp GP2Y0A21YK (arduino analog pin 2)
   Connections: see vid
   See you.tube vid: https://youtu.be/-1Vl_qlzZng
   Based on  Arduino controlled light dimmer  at instructables.com
   http://www.instructables.com/id/Arduino-controlled-light-dimmer-The-circuit/?ALLSTEPS
   And
   Curso Arduino Advanced - Aula 14 (TRIAC) renatoaloi.blogspot.com
   https://www.youtube.com/watch?v=CmU2u2kHH1U&index=32&list=PL05085BAC19CB5DF1
   And
   Digital AC Dimmer Module
   http://www.inmojo.com/store/inmojo-market/item/digital-ac-dimmer-module/
*/

/*

  AC Light Dimmer v.2 - Inmojo
  AC Voltage dimmer with Zero cross detection

  Author: Charith Fernanado http://www.inmojo.com charith@inmojo.com
  License: Released under the Creative Commons Attribution Share-Alike 3.0 License.
  https://creativecommons.org/licenses/by-sa/3.0/
  Target:  Arduino

  Attach the Zero cross pin of the module to Arduino External Interrupt pin
  Select the correct Interrupt # from the below table

  Pin    |  Interrrupt # | Arduino Platform
  ---------------------------------------
  2      |  0            |  All
  3      |  1            |  All
  18     |  5            |  Arduino Mega Only
  19     |  4            |  Arduino Mega Only
  20     |  3            |  Arduino Mega Only
  21     |  2            |  Arduino Mega Only

  Please select your utility power frequency from frq variable.
  NOTE:
  Let me just reiterate the statement: This program is a demo of how you can
  control the dimmer. It is NOT and efficient program as it spends most of its time
  waiting. It is therefore NOT the most suitable to combine with other tasks of the
  processor. If you need a more efficient program use a timer instead of delay

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License v3 as published by
  the Free Software Foundation.
*/
#include <TimerOne.h>   // inclui biblioteca Timer1
#include <MsTimer2.h>   // Inclui biblioteca MsTimer2
#define  fullOn    10
#define  fullOff   127
#define  FQ_50      1 // in case of 50Hz
#define  FQ_60      0 // in case of 50Hz
#define  VER       "2.0"
int reading = 65;
int sensorDistance = A1;
int inbyte;
int AC_LOAD = 3;    // Output to Opto Triac pin
int dimming = fullOff;  // Dimming level (0-128)  0 = ON, 128 = OFF
int distance = 0;
boolean frq = FQ_60;     // change the frequency here.
boolean countUp = false;
boolean simulate = true;

void setup()
{
  Serial.begin(9600);
  pinMode(AC_LOAD, OUTPUT);	      // Set the AC Load as output
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
  //Serial.begin(115200);

  MsTimer2::set(200, readSensor); // inicializa Timer2 com 200 milisegundos (tempo entre leituras do sensor de dintancia)
  MsTimer2::start();  // start do timer2
}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation
  // 50Hz-> 10ms (1/2 Cycle) → (10000us - 10us) / 128 = 78 (Approx)
  // 60Hz-> 8.33ms (1/2 Cycle) → (8333us - 8.33us) / 128 = 65 (Approx)

  delayMicroseconds(65 * dimming);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(8.33);         // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off

}

void loop()
{

  dimming = reading;
  delay(100);

}

void readSensor() {
  distance = analogRead(sensorDistance);  // reads distance sensor
  Serial.print(distance); // for debug, uncomment
  distance = map(distance, 0, 600, 127, 0); //maps acquired distance for a power range between 0 and 120
  reading = constrain(distance, 0, 120);
  Serial.print("/"); // for debug, uncomment
  Serial.println(reading); // for debug, uncomment
}
