/* Project Ardu_Serie # 47
  J3C3 - Controlling Motor - Techniques For Driving Inductive Load  - L9110S
   Code III
   Ino File: _47_L9110_test_motor_03.ino
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
    MOTOR
    VCC -> 3
    GND -> GND

    POT
    TRIG -> A0
    vcc and gnd

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

const byte motorPin = 3;
const byte potPin = A0;         //although 0 will work as well it's more clear to use the Ax notation

void setup () {
  pinMode(motorPin,OUTPUT);
  pinMode(potPin,INPUT);
  Serial.begin(9600);
}

void loop () {
  int sensorValue;
  sensorValue = analogRead(potPin);
  delay(100);
  float digitalValue = map(sensorValue, 0, 1023, 0, 255);
  Serial.println(digitalValue);
  analogWrite(motorPin,digitalValue);
  
}
