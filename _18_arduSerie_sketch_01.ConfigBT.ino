/*
  Project name:
     18 #arduSerie - Arduino & MIT App Instaler 2 & HC-06 
     (Smart Phone App!)
     Flavour I - 
     Hex File: _18_arduSerie_sketch_01.ConfigBT.ino
   Revision History:
     20161008:
       - board found on Arduino e Cia Examples
      (connect everything on HC-06 to access smart phone)
      
   Description:
        A Simple bluetooth config ardu_sketch to HC-06 setup!!!
        Please goto lib manager and download SoftwareSerial!!!
        
   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
   IC Chip:              Bluetooth Datasheet - HC-06   https://goo.gl/s9ZcNC
  
   Connections:
      See Official youtube channel vids: https://youtu.be/u1xqW6qtFXs
      


   Based on:   Módulo Bluetooth JY-MCU - HC-06 - Configuração (Arduino e Cia)
   https://goo.gl/W13Qnn
   Code by Arduino e Cia 
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundation
*/
#include <SoftwareSerial.h>  
   
   
SoftwareSerial mySerial(10, 11);        // RX, TX  
String command = "";                    // Stores response of bluetooth device  
                                        // which simply allows \n between each  
                                        // response.  
   
void setup()   
{  
                                        // Open serial communications and wait for port to open:  
  Serial.begin(9600);                   // Default baudrate firmware 
  //Serial.begin(115200);               // I changed to this baudrate...
  Serial.println("Type AT commands!");  
                                        // SoftwareSerial "com port" data rate. JY-MCU v1.03 defaults to 9600.  
  mySerial.begin(9600);
  //mySerial.begin(115200);             // ...and do not forget to match baudrate
}  
   
void loop()  
{  
                                        // Read device output if available.  
  if (mySerial.available()) 
  {  
     while(mySerial.available()) 
     {                                  // While there is more to be read, keep reading.  
       command += (char)mySerial.read();  
     }  
   Serial.println(command);  
   command = "";                        // No repeats  
  }  
  
                                        // Read user input if available.  
  if (Serial.available())
  {  
    delay(10);                          // The DELAY!  
    mySerial.write(Serial.read());  
  }  
   
}                                       // END loop()  
