/* Project Ardu_Serie # 47
  J3C3 - Controlling Motor - Techniques For Driving Inductive Load  - L9110S
   Code I
   Ino File: _47_L9110_test_motor_01.ino
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

const int pwm = 3;
const int dir = 4;

void setup() {
  pinMode(dir, OUTPUT);
  pinMode(pwm, INPUT);     // Optional: default is input
}

void loop() {
  DC (255, HIGH);          // change to LOW and reverse direction
}

void DC (unsigned char p, unsigned char d)
{
  digitalWrite(dir, d);
  if (d == LOW)
  {
    analogWrite(pwm, p);
  }
  else
  {
    analogWrite(pwm, 255 - p);
  }
}
