/*
  Project name:
     08 # arduSerie - Ardu_Tacho Meter v1.0
     Flavour I
     Hex File: _08_arduSerie_ArduTachoMeterV10_I.ino
   Revision History:
     20162705:
       - found on internet (gilj3)
     http://bildr.org/2011/06/qre1113-arduino/
      Description:
      To measure the angular velocity in RPM we can use phototransistor and Schmitt Trigger; 
      the chip is used to pass the analog signal to digital and so we read the result
      on the PIN 4 of the Arduino. The Schmitt Trigger is very cool!!! check over !!!

  MCU:              Arduino - @16MHz  http://www.arduino.cc/
  IC Chip:          M74HC14B1 - Schmitt Trigger
 https://en.wikipedia.org/wiki/Schmitt_trigger
 Connections:
     you.tube vid:  https://youtu.be/mesFtKku4bU
   Based on http://bildr.org/
   Tutorial:Line Sensing. QRE1113 Reflectance Sensor + Arduino

 WEB: http://jungletronics.blogspot.com.br/2016/05/08-arduserie-ardu-tacho-meter-v10.html
 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

int encoder_pin = 2;                    // pulse output from the module
unsigned int rpm;                       // rpm reading
volatile byte pulses;                   // number of pulses
unsigned long timeold;
                                        // number of pulses per revolution
                                        // based on your encoder disc
unsigned int pulsesperturn = 2;
void counter()
{
                                        //Update count
   pulses++;
}
void setup()
{
   Serial.begin(9600);
   pinMode(encoder_pin, INPUT);
                                        //Interrupt 0 is digital pin 2
                                        //Triggers on Falling Edge (change from HIGH to LOW)
   attachInterrupt(0, counter, FALLING);
                                        // Initialize
   pulses = 0;
   rpm = 0;
   timeold = 0;
}
void loop()
{
   if (millis() - timeold >= 1000) {
                                        //Don't process interrupts during calculations
      detachInterrupt(0);
      rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses;
      timeold = millis();
      pulses = 0;
      Serial.print("RPM = ");
      Serial.println(rpm,DEC);
                                        //Restart the interrupt processing
      attachInterrupt(0, counter, FALLING);
   }
}
