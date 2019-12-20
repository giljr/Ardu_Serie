/* Project: Ardu_Series # 88
 *   Binary Counter - Logic Analyzer - Building an LED Binary Counter & Improving The Code
 *   
 *   INO file: _88_logicCounter_PaulMcWhorter_4bits_00.ino
 *   
 *   Date: Dez 2019
 *  
 *   Code by: http://www.toptechboy.com/
 *   Hardware by: Paul McWhorter
 * 
 *   Description: Let's count until 16? this is an easy way to understand binary counter 
 *                forever using Arduino! First let's test the hardware:)
 *  
 *   Visit: Jungletronics: https://medium.com/jungletronics
 *  
 *   Video Tutorial from Mr. McWhoter: https://youtu.be/Lg39qKrdySU
 *  
 *  License: CC-SA 3.0, feel free to use this code however you'd like.
 *  Please improve upon it! Let me know how you've made it better.
 */
                            // Declaring pin's variables
int pin1=1;
int pin2=2;
int pin3=3;
int pin4=4;

void setup() {
                            // Setting all pins as output
pinMode(pin1, OUTPUT);
pinMode(pin2, OUTPUT);
pinMode(pin3, OUTPUT);
pinMode(pin4, OUTPUT);
}

void loop() {
                            // Run it all all (LEDs are on forever - Test Purpose)
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);

}
