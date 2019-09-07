/* Project -  Ardu_Serie#62
   L9110 H-bridge module - DC Stepper Motor Driver Board -.8A@12 v peak 
   
   Code II
   
   Ino File: _62_L9110_For_Two_Motor_01.ino
   
   Use: L9110S - https://www.elecrow.com/download/datasheet-l9110.pdf
   
   Objective: HHello, this is a simple test of this module. I am using 2 motors of 
   12 volts, 350 RPM. The power supply is 9 volts 500 mA
   
   Connections:
    LM9110 & MOTOR
    A -> 3
    B -> 4
    vcc in vin & gnd (use external power 9v .5A)
   
   Created: Oct/2018
   Author : J3
   WebPage:
   Terms of use:
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/


const int AIA = 3;
const int AIB = 4;
const int BIA = 5;            // If you have the dual driver remove
const int BIB = 6;            // the comments and enable the other engine
byte speed = 255;

void setup() {
  Serial.begin(9600);
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
}
void loop() {
  forward();                    // Forward and back forever!
  delay(4000);
  backward();
  delay(4000);

}
void backward()
{
  analogWrite(AIA, 0);          // Simply reverse the logic ;-)
  analogWrite(AIB, speed);      // Watch the video to grab the knowledge!
  analogWrite(BIA, 0);
  analogWrite(BIB, speed);
}
void forward()
{
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
}

