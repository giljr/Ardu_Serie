/*
  Project name:
     16 #arduSerie - Arduino Headless Setup  & ESP 8266 
     (WIFI cheap chip!)
     Flavour III - 
     Hex File: _16_arduSerie_sketch_03.WiFiWebServer.ino
   Revision History:
     20160927:
       - board found on Arduino IDE Exemples
     (connect everything on ESP8266 to access examples)
   Description:
     This sketch demonstrates how to set up a simple HTTP-like server.
     The server will set a GPIO pin depending on the request
     http://server_ip/gpio/0 will set the GPIO2 low,
     http://server_ip/gpio/1 will set the GPIO2 high
     server_ip is the IP address of the ESP8266 module, will be 
     printed to Serial when the module is connected.
     By accessing GPIO2 and put values 1(high) and 0(low) to change the 
     status of the led on pin GPIO2 on ESP8266 module 01.

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

#include <ESP8266WiFi.h>

const char* ssid = "colibri";
const char* password = "**********"; //change it!

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
