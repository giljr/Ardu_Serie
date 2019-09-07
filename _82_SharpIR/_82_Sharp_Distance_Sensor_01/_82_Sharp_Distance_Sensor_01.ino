/*  Project: Ardu_Serie # 82
     Sharp IR Sensor -  0A41SK
     4–30cm Infrared IR Distance Measuring Sensor

     INO file: _82_Sharp_Distance_Sensor_01.ino

     date: 8/27/19

    code by: https://github.com/guillaume-rico
    hardware by: GP2Y0A02YK0F IRSharp (https://www.pololu.com/product/2464)
       software: Arduino IDE 1.8.9

    Description: There is no easy way more simple than that sketch to test right away GP2Y0A02YK0F IRSharp!
                 Please open Serial Monitor at 9600 bps:)

    Visit: https://medium.com/jungletronics
           SHARP GP2Y0A710K0F IR sensor with Arduino and SharpIR library example code
           More info: https://www.makerguides.com

    Tutorial: https://medium.com/jungletronics/sharp-ir-sensor-0a41sk-41c2b3848e39

    License: CC-SA 3.0, feel free to use this code however you'd like.
    Please improve upon it! Let me know how you've made it better.
*/
/**/
// Include the library:
#include <SharpIR.h>
// Define model and input pin:
#define IRPin A5
#define model 430

// Create variable to store the distance:
int distance_cm;
/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F --> 430
*/
// Create a new instance of the SharpIR class:
SharpIR mySensor = SharpIR(IRPin, model);
void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
}
void loop() {
  // Get a distance measurement and store it as distance_cm:
  distance_cm = mySensor.distance();
  // Print the measured distance to the serial monitor:
  Serial.print("Mean distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(1000);
}
