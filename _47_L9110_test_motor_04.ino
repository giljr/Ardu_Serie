/* Project Ardu_Serie # 47
  J3C3 - Controlling Motor - Techniques For Driving Inductive Load  - L9110S
   Code IV
   Ino File: _47_L9110_test_motor_04.ino
   Use: L9110S - https://www.elecrow.com/download/datasheet-l9110.pdf

   Objective: Hi, What are the best options for controlling motors with inductive loads?
   Here I invastigate L9110S - Motor control driver chip;)

   Instructions (CODE SEQUENCE)
      (1) The control: simply the DC MOTOR, L9110 and Arduino;
      (2) We add the movement forward and back frozen in the code;
      (2) We add a potentiometer to vary the speed; no L9110 chip; single direction;
      (4) We add the potentiometer and the state machine; L9110 chip returns:)
      (5) We refined the code to present a menu, a la carte!; That's all!

   Connections:
    LM9110 & MOTOR
    A -> 3
    B -> 4
    vcc in vin & gnd (use external power 9v)
    POT
    TRIG -> A0
    vcc & gnd

   Note:
    Whenever possible add complexity gradually to your code:)

   Created: Sept/2018
   Author : J3
   WebPage:

   Base on: Control de Motores - Que puente en H usar? by J.Aguirre
            https://youtu.be/rXAxxvEy-fc

   Terms of use:
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
const byte potPin = A0;          // Pot's pin

int STATE = 0;

const int AIA = 3;
const int AIB = 4;
//const int BIA = 5;
//const int BIB = 6;
byte speed = 255;

void setup() {
  Serial.begin(9600);
  pinMode(potPin, INPUT);

  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  //pinMode(BIA, OUTPUT);
  //pinMode(BIB, OUTPUT);
}
void loop() {
  int sensorValue = analogRead(potPin);
  if (sensorValue > 512)
    STATE = 1;
  if (sensorValue < 513)
    STATE = 2;

  //forward();                       // control by state machine
  //delay(4000);
  //backward();
  //delay(4000);

  switch (STATE)
  {
    case 1:
                                     // do something when var equals 1
      Serial.println("FORWARD");
      speed = map(sensorValue, 513, 1023, 0, 255);
      //Serial.print("sensorValue= ");
      //Serial.println(sensorValue);
      //analogWrite(motorPin, speed);
      forward();
      delay(1000);
      

      break;
    case 2:
                                     // do something when var equals 2
      Serial.println("REVERSE");
      speed = map(sensorValue, 512, 0, 0, 255);
      //Serial.print("sensorValue= ");
      //Serial.println(sensorValue);
      //analogWrite(motorPin, speed);
      backward();
      delay(1000);
      break;
    default:
                                     // if nothing else matches, do the default
                                     // default is optional
      delay(10);
  }
}
void backward()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, speed);
  //analogWrite(BIA, 0);
  //analogWrite(BIB, speed);
}
void forward()
{
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  //analogWrite(BIA, speed);
  //analogWrite(BIB, 0);
}

