/*
   Project name:
     05 # arduSerie - DD_ARD_YU v1.0 - Digital Dimmer ARDuino for YoU
     (The 'Hello World' example for TRIAC)
    flavour I
    Arduino File: Dimmer_Digital.ino
    Revision History:
     20160502:
   Description:
     We will discuss the operation of TRIAC, MOC and 4n25 in its essence. We bring several graphics
     for better understanding of the basic concepts. Let's master the alternating current.
     Be careful, be conscious of the dangers of handling high-voltage currents.
     You can kill yourself !!! Read license, please !!!

   MCU:                Arduino - @16MHz   http://www.arduino.cc/
   IC Chip:            MOC3021, TIC246B, BRIDGE RET W10M, 4N25
   Connections:  see vid
   See you.tube vid: https://youtu.be/-1Vl_qlzZng
   Based on  Arduino controlled light dimmer  at instructables.com
   http://www.instructables.com/id/Arduino-controlled-light-dimmer-The-circuit/?ALLSTEPS
   And
   Curso Arduino Advanced - Aula 14 (TRIAC) renatoaloi.blogspot.com
   https://www.youtube.com/watch?v=CmU2u2kHH1U&index=32&list=PL05085BAC19CB5DF1
   And
   Digital AC Dimmer Module
   http://www.inmojo.com/store/inmojo-market/item/digital-ac-dimmer-module/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation.
*/


int UP = 7;
int DOWN = 8;
int LAMP = 3;
int dimming = 128;
int counts = 7;
int dimmer[7] = { 1, 105, 100, 75, 50, 25, 10 };
int i = 0;

void setup()
{
  //Serial.begin(9600);
  //delay(1000);
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LAMP, OUTPUT);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
  attachInterrupt(0, zero_crosss_int, RISING);
}

void loop()
{
  if (!digitalRead(UP) || !digitalRead(DOWN))
  {
    if (check_button(UP))
    {
      if (i < (counts - 1)) i++;

    }
    else if (check_button(DOWN))
    {
      if (i > 0) i--;

    }
  }

  dimming = dimmer[i];
  delay(100);
}

void zero_crosss_int()
{
  int dimtime = (65 * dimming);
  delayMicroseconds(dimtime);
  digitalWrite(LAMP, HIGH);
  delayMicroseconds(8.33);
  digitalWrite(LAMP, LOW);
}

byte check_button(byte button)
{
  if (!digitalRead(button))
  {
    unsigned long time1 = millis() + (150 - dimming);
    while (time1 > millis())
    {
      if (digitalRead(button)) return LOW;
    }
    return HIGH;
  }
  else
    return LOW;
}
