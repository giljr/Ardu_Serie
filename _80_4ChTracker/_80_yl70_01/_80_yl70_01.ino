/*  Project: Ardu_Serie # 80
     4-Channel Line Tracker Sensor
     Ensures a Successful & Accurately Navigation For Your Robot

     INO file: _80_yl70_01.ino

     date: 8/24/19

    code by: http://www.energiazero.org
    hardware by: Walfront

    Description: There is no easy way to tell your robot where to go! use this module.

    Visit: https://medium.com/jungletronics

    Tutorial: https://medium.com/jungletronics/4-channel-line-tracker-sensor-e6f9a0dad304

    License: CC-SA 3.0, feel free to use this code however you'd like.
    Please improve upon it! Let me know how you've made it better.
*/
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  Serial.print(digitalRead(2));
  Serial.print(" ");
  Serial.print(digitalRead(3));
  Serial.print(" ");
  Serial.print(digitalRead(4));
  Serial.print(" ");
  Serial.println(digitalRead(5));
  delay(500);
}
