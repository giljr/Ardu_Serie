/* Project Ardu_Serie # 48
  EASYDRIVER: 4-Wire-Stepper Motor Driver - Brian Schmalz Design on A3967 IC 
              Bi-Polar Motors — .75A@30v peak — Making Using This a Breeze!
   Code V
   Ino File: _48_EASYDRIVER_test_motor_01.ino
   From: Brian Schmalz - Easy Driver Examples - Sample code and projects to get your stepper running!
   http://www.schmalzhaus.com/EasyDriver/Examples/EasyDriverExamples.html

   Objective: Hi,This is the most basic example you can have with an Arduino,
              an Easy Driver, and a stepper motor;)
   Connections:
    STEPPER MOTOR 28BYJ-48 5V
    https://www.digikey.com/catalog/en/partgroup/28byj-48-series/59691

    Coil B -> Pink + Orange
    Coil A -> l A
    vcc in vin & gnd (use external power 9v .5a)

   Created: Oct/2018
   Author : Brian Schmalz
   WebPage: https://medium.com/jungletronics/easydriver-4-wire-stepper-motor-driver-9f32b233efe6

   Base on: Brian Schmalz Example

   Terms of use:
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/


void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void loop() {
  digitalWrite(9, HIGH);
  delay(1);
  digitalWrite(9, LOW);
  delay(1);
}
