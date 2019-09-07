/*
  Project name:
     08 # arduSerie - Ardu_Tacho Meter v1.0
     Flavour II
     Hex File: _08_arduSerie_ArduTachoMeterV10_II.ino
   Revision History:
     20162705:
       - found on internet (gilj3)
     http://bildr.org/2011/06/qre1113-arduino/
      Description:
      Test phototransistor tp850; breadboard with 74hc1401 - schmitt trigger;
      The input value is reversed in output; Outputs via the serial terminal; 
      0 mean Obstruction (4.28v) and 1 means no obstruction (0.08v). Remember, 
      results are inverted because the schmitt trigger!!!
  MCU:              Arduino - @16MHz  http://www.arduino.cc/
  IC Chip:          Reflective Optical Sensors TP850
  http://pt.aliexpress.com/item/10PCS-TP850/32379353983.html?spm=2114.02010208.3.9.pHMwtD&ws_ab_test=searchweb201556_0,searchweb201602_5_10017_10021_507_10022_10020_10009_10008_10018_10019_101,searchweb201603_3&btsid=b25336a1-0bc8-42f4-b252-4bebc7e6f914
  Connections:
     you.tube vid:  https://youtu.be/mesFtKku4bU
   Based on http://bildr.org/2011/06/qre1113-arduino/
   Tutorial:Line Sensing. QRE1113 Reflectance Sensor + Arduino
  WEB: http://jungletronics.blogspot.com.br/2016/05/08-arduserie-ardu-tacho-meter-v10.html
  
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License v3 as published by
   the Free Software Foundatio
*/

int TP850_Pin = 2; //connected to digital 2

void setup() {
  Serial.begin(9600);
}


void loop() {

  int TP_Value = digitalRead(TP850_Pin);
  Serial.println(TP_Value);
 delay(100);

}
