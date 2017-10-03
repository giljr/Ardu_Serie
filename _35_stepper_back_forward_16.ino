/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour XVI - Working with Stepper Motor
     INO File: _35_stepper_back_forward_16.ino
   Revision History:
     Sept, 2017
       - v 1.0 - Medium webpage: https://goo.gl/R25hLc  & https://goo.gl/dReMhG
   Description:
         Here we tested the engine forwards and backwards. Very basic!
         For J3BCP based on http://www.brokking.net codes.
         We accept collaborations:D! Visit our webpage!
   Stepper Motor:        Nema 17 - 3,2Kgf.cm - JK42HS34-0424A
                         http://www.soldafria.com.br/motor-de-passo-nema-17-32kgfcm-jk42hs34-0424a-p-8043.html
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   IMU:                  9-DOF IMU module - GY-85
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
   Credit:               Joop Brokking
   Data Sheet ITG3205:   http://www.datasheetspdf.com/PDF/ITG3205/731955/1
   Terms of use:
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
     THE SOFTWARE.
*/
#define DIR_L   (5)           // Left Motor
#define STEP_L  (4)

#define DIR_R   (3)           // Right Motor
#define STEP_R  (2)

int dPause    =  1000;       // Fast move
//int dPause    =  10000;       // Slow move

int dirLevel  = HIGH;

int angle_roll_acc;          // Simulating angle values

/**********************************************************/

void setup() {
  pinMode(DIR_L, OUTPUT);
  pinMode(STEP_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(STEP_R, OUTPUT);

  TCCR2A = 0;
  TCCR2B = 2;
  TIMSK2 = 1 << TOIE2;
  TCNT2 = 0xD9;
}

//-------- LOOP  -----//

void loop()
{
  //empty for now:)
}

void stepGo() {
  digitalWrite(STEP_L, HIGH);
  digitalWrite(STEP_R, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP_L, LOW);
  digitalWrite(STEP_R, LOW);
  delayMicroseconds(dPause);
}

ISR(TIMER2_OVF_vect) {
  angle_roll_acc ++;


  if ( angle_roll_acc % 100 == 0 ) {
    if ( dirLevel == HIGH ) dirLevel = LOW;
    else                    dirLevel = HIGH;
  }



  digitalWrite( DIR_L, dirLevel );          // As the motors are mounted in reverse,
  digitalWrite( DIR_R, !dirLevel );        //  then reverse the direction here as well.
  stepGo();
}
