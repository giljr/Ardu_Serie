/*
  Project name:
     16 #arduSerie - Arduino Headless Setup  & ESP 8266 
     (WIFI cheap chip!)
     Flavour I - 
     Hex File: _16_arduSerie_sketch_01.HelloServer_Json.ino
   Revision History:
     20160927:
       - board found on Arduino IDE Exemples
      (connect everything on ESP8266 to access examples)
      
   Description:
        Simple web page that sends messages to two addresses urls.
        It is a hello world of webserver to ESP8266!!!
        
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
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "colibri";
const char* password = "**********";//change it!!!

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "application/json", "{test: test}");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
