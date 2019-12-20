/* Project: Ardu_Series # 88
 *   Binary Counter - Logic Analyzer - Building an LED Binary Counter & Improving The Code
 *   
 *   INO file: _88_logicCounter_PaulMcWhorter_4bits_02.ino
 *   
 *   Date: Dez 2019
 *  
 *   Code by: http://www.toptechboy.com/
 *   Hardware by: Paul McWhorter
 * 
 *   Description: This improved code counts from 0 until 15 (4-bit counter); 
 *                See how easy is to test with other delays:)
 *  
 *   Visit: Jungletronics: https://medium.com/jungletronics
 *  
 *   Video Tutorial from Mr. McWhoter: https://youtu.be/Lg39qKrdySU
 *  
 *  License: CC-SA 3.0, feel free to use this code however you'd like.
 *  Please improve upon it! Let me know how you've made it better.
 */
                                     // Declaring variables
int pin1=1;
int pin2=2;
int pin3=3;
int pin4=4;
int waitTime=350;                    // Delay time

void setup() {
                                     // All pins are output
pinMode(pin1, OUTPUT);
pinMode(pin2, OUTPUT);
pinMode(pin3, OUTPUT);
pinMode(pin4, OUTPUT);
}

void loop() {
                                    // Repeating this patters 16 times
  digitalWrite(pin1, LOW);          // Make the counter on a piece of paper...
  digitalWrite(pin2, LOW);          //  Dec  - Bin
  digitalWrite(pin3, LOW);          //  00   - 0000 
  digitalWrite(pin4, LOW);          //  01   - 0001
  delay(waitTime);                       //  02   - 0010
                                    // (...)
  digitalWrite(pin1, HIGH);         // 15   - 1111 
  digitalWrite(pin2, LOW);          // 
  digitalWrite(pin3, LOW);          // ...then transfer to code:)
  digitalWrite(pin4, LOW);          // Easy, right?
  delay(waitTime);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  delay(waitTime);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
  delay(waitTime);
  
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  delay(waitTime);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  delay(waitTime);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  delay(waitTime);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  delay(waitTime);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(waitTime);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(waitTime);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(750);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(waitTime);
  
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  delay(waitTime);

  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  delay(waitTime);

  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  delay(waitTime);
  
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
  delay(750);

}
