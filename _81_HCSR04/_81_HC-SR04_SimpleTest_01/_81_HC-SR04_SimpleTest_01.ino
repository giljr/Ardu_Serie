/*   Project: Ardu_Serie # 81
     HC-SR04 Ultrasound sensor 4-Arduino
     Ultra Simple Test Experiment

     INO file: _81_HC-SR04_SimpleTest_01.ino

     date: 8/25/19

    code by: https://forum.arduino.cc
    hardware by: Arduino IDE 1.8.9

    Description: There is no easy way more simple than that sketch to test right away HC-SR04!
                 Please open Serial Monitor at 9600 bps:)

    Visit: https://medium.com/jungletronics

    Tutorial: https://medium.com/jungletronics/hc-sr04-ultrasound-sensor-4-arduino-3cb60b673ad0

    License: CC-SA 3.0, feel free to use this code however you'd like.
    Please improve upon it! Let me know how you've made it better.
*/
/*
  HC-SR04 Ping distance sensor]
  VCC to arduino 5v GND to arduino GND
  Echo to Arduino pin 13 Trig to Arduino pin 12
*/

int echoPin = 13;
int trigPin = 12;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int duration, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 29 / 2;
  /* Sound velocity=29cm/microsec,
    then divide by 2 because of the return time
  */
  Serial.print(cm);
  Serial.println(" cm");
  delay(500);
}
