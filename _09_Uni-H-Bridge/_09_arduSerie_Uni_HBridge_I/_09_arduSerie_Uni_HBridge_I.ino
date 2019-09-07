/*
  Project name:
     09 # arduSerie - Uni H-Bridge Module For Arduino
     Flavour I
     Hex File: _09_arduSerie_Uni_HBridge_I.ino
   Revision History:
     20160603:
      Description:
      In this code we use the i2c module to facilitate connections
      with LCD and potentiometer to serve as the engine throttle. 
      By pressing the button the LCD shows what was the biggest 
      rotation per minute - RPM -  recorded. This module is for an 
      engine only with which we can control the direction and speed 
      of a DC motor.It uses a L9110 Chip that is widely used in toy 
      car motor drives,stepper motor drive and switching power 
      tube circuit. It is very cool chip!!! check it over !!!

  MCU:              Arduino - @16MHz  http://www.arduino.cc/
  IC Chip:          L9110 dual-channel h-bridge motor driver module
 http://www.elecrow.com/download/datasheet-l9110.pdf
 Library: New LiquidCrystal 
 https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home#!lcd-library
 TimeOne:
 http://playground.arduino.cc/Code/Timer1
 Connections:
     you.tube vid: https://youtu.be/J3uOs2HWvRI 
   Based on http://tronixlabs.com.au/news/tutorial-serial-i2c-backpack-for-hd44780compatible-lcd-modules-with-arduino/
   Tutorial - Serial I2C backpack for HD44780-compatible LCD modules with Arduino and http://playground.arduino.cc/Code/Timer1
 Web: http://jungletronics.blogspot.com.br/2016/06/09-arduserie-uni-h-bridge-module-for_3.html
 
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

#include <TimerOne.h>
//http://playground.arduino.cc/Code/Timer1
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home#!lcd-library

LiquidCrystal_I2C  lcd(0x27, 2, 1, 0, 4, 5, 6, 7);   // 0x27 is the I2C bus address for an unmodified backpack

int b1a = 6;                                         // L9110 B-1A
int b1b = 9;                                         // L9110 B-1B
volatile unsigned int  rotation;
volatile unsigned int counter;
unsigned int rpm=0, maxRPM=0;                        // store RPM readings
int but = 3;                                         // button to control when to display MAX RPM 
int pot = 1;                                         // potentiometer connected to Pin A1

void docount()                                       // counts from the speed sensor
{
  counter++;                                         // increase +1 the counter value
}

void timerIsr()
{
  Timer1.detachInterrupt();                          // stop the timer to instant calculation
  rotation = (counter / 2);                          // divide by number of holes in Disc
  counter = 0;                                       //  reset counter to zero
  Timer1.attachInterrupt( timerIsr );                // enable the timer
}

void setup()
{

  pinMode(but, INPUT);
  digitalWrite(but, LOW);
  
                                                     // activate LCD module
  lcd.begin (16, 2);                                 // for 16 x 2 LCD module
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);

  Serial.begin(9600);

  pinMode(b1a, OUTPUT);
  pinMode(b1b, OUTPUT);

  Timer1.initialize(1000000);                        // set timer for 1sec
  attachInterrupt(0, docount, RISING);               // increase counter when speed sensor pin goes High
  Timer1.attachInterrupt( timerIsr );                // enable the timer
}

void loop()
{
  lcd.clear();
  lcd.home ();                                       // set cursor to 0,0
  lcd.print(rotation);
  lcd.print(" / ");
  rpm = rotation * 60;
  if (rpm > maxRPM) {                               // store the Max Rpm
    maxRPM = rpm;
  }
  lcd.print(rpm);
  lcd.setCursor (0, 1);                              // go to start of 2nd line
  lcd.print("Rps / Rpm :Speed ");
  
 
  if (digitalRead(but)== HIGH)                       // if you push the button to show the maximum rpm
                                                   
  {
    lcd.clear();
    lcd.home (); 
    lcd.print("MAXIMUM RPM");
    lcd.setCursor(0, 1);
    lcd.print(maxRPM);
    lcd.print(" Rpm");
  }
  delay(1000);
  int potvalue = analogRead(pot);                    // potentiometer connected to Pin A1
  int motorspeed = map(potvalue, 0, 680, 255, 0);
  analogWrite(b1a, motorspeed);                      // set speed of motor (0-255)
  digitalWrite(b1b, 1);                              // set rotation of motor to Clockwise
                                                     // controlling the variables you can 
                                                     //set in the opposite direction ...
}
