
/*
  Project name:
     HowTo Control devices with PIDLibrary —  #arduSerie 23
     Flavour VII - Final Result
     Hex File: _23_arduSerie_ArduTachoMeterV2_VII.ino
   Revision History:
     20161226:
   Description:
       this is the goal: we have to control the speed of an exhaust fan by
       the pid method. We will use the Arduino PID Library by Brett Beauregard
       and Front-End v03 using Processing.org version 3.1.

   MCU:              Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   74HC14            IC Chip - SChmitt Trigger     https://en.wikipedia.org/wiki/Schmitt_trigger
   Connections:
      See Officil you.tube channel vid:
   Based on:   Code by Brett Beauregard

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation

*/
/******************************************************************
   PID Simple Example (Augmented with Processing.org Communication)
   Version 0.3
   by Brett Beauregard
   License: Creative-Commons Attribution Share-Alike
   April 2011
 ******************************************************************/

/********************************************
   Functions for pid configuration
 ********************************************/

#include <PID_v1.h>
//Define Variables we'll be connecting to
double Setpoint, Input, Output;
int inputPin = 0, outputPin = 9;
int val = 0;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, .10, .01, .00, DIRECT);

/********************************************
   Functions for 16x2 IIC LCD configuration
 ********************************************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 5, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display


/********************************************
   This will help us know when to talk with 
   processing
 ********************************************/
unsigned long serialTime;
int encoder_pin = 2;                    // pulse output from the module
unsigned int rpm;                       // rpm reading
volatile byte pulses;                   // number of pulses
unsigned long timeold;
// number of pulses per revolution
// based on your encoder disc
unsigned int pulsesperturn = 2;

/********************************************
   Interruption Routine
 ********************************************/
void counter()
{
  //Update count
  pulses++;
}

void setup()
{
  //initialize the serial link with processing
  Serial.begin(9600);

  lcd.init();                      // initialize the lcd
  //lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(0,0);
  //lcd.print("Hello, world!!!!");
  //lcd.setCursor(0,1);
  //lcd.print("RPM");

  pinMode(encoder_pin, INPUT);
  //Interrupt 0 is digital pin 2
  //Triggers on Falling Edge (change from HIGH to LOW)
  attachInterrupt(0, counter, FALLING);
  // Initialize
  pulses = 0;
  rpm = 0;
  timeold = 0;

  //initialize the variables we're linked to
  //Input = analogRead(inputPin);
  //Input = rpm;
  Setpoint = 3000;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop()
{

  if (millis() - timeold >= 1000) {
    //Don't process interrupts during calculations
    detachInterrupt(0);
    rpm = (60 * 1000 / pulsesperturn ) / (millis() - timeold) * pulses;
    timeold = millis();
    pulses = 0;
    //Restart the interrupt processing
    attachInterrupt(0, counter, FALLING);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(rpm, DEC);
    lcd.setCursor(0, 1);
    lcd.print("RPM");

  }

  Input = rpm;
  //pid-related code
  //Input = analogRead(inputPin);
  myPID.Compute();
  analogWrite(outputPin, Output);

  //send-receive with processing if it's time
  if (millis() > serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime += 500;
  }


}

/********************************************
   Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array



// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Direct, 1=Reverse, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output
//  14-17: float P_Param
//  18-21: float I_Param
//  22-245: float D_Param
void SerialReceive()
{

  // read the bytes sent from Processing
  int index = 0;
  byte Auto_Man = -1;
  byte Direct_Reverse = -1;
  while (Serial.available() && index < 26)
  {
    if (index == 0) Auto_Man = Serial.read();
    else if (index == 1) Direct_Reverse = Serial.read();
    else foo.asBytes[index - 2] = Serial.read();
    index++;
  }

  // if the information we got was in the correct format,
  // read it into the system
  if (index == 26  && (Auto_Man == 0 || Auto_Man == 1) && (Direct_Reverse == 0 || Direct_Reverse == 1))
  {
    Setpoint = double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the
    //   value of "Input"  in most cases (as
    //   in this one) this is not needed.
    if (Auto_Man == 0)                    // * only change the output if we are in
    { //   manual mode.  otherwise we'll get an
      Output = double(foo.asFloat[2]);    //   output blip, then the controller will
    }                                     //   overwrite.

    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    myPID.SetTunings(p, i, d);            //

    if (Auto_Man == 0) myPID.SetMode(MANUAL); // * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //

    if (Direct_Reverse == 0) myPID.SetControllerDirection(DIRECT); // * set the controller Direction
    else myPID.SetControllerDirection(REVERSE);          //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
void SerialSend()
{
  Serial.print("PID ");
  Serial.print(Setpoint);
  Serial.print(" ");
  Serial.print(Input);
  Serial.print(" ");
  Serial.print(Output);
  Serial.print(" ");
  Serial.print(myPID.GetKp());
  Serial.print(" ");
  Serial.print(myPID.GetKi());
  Serial.print(" ");
  Serial.print(myPID.GetKd());
  Serial.print(" ");
  if (myPID.GetMode() == AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");
  Serial.print(" ");
  if (myPID.GetDirection() == DIRECT) Serial.println("Direct");
  else Serial.println("Reverse");
}
