/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour II - Working with Stepper Motor
     Hex File: _35_ProMini_Two_StepperMotor_13.ino
   Revision History:
     Aug, 2017
       - Medium webpage: https://goo.gl/gHFGIi   &  https://goo.gl/rdL4uY
   Description:
         Code to test the two stepper motor for J3BC project.
         The series of several text and codes are based on the work of Joop Brokking
         who published an Awesome project name 'Your Arduino Balancing Robot (YABR)
   Motor:                12V NEMA 17 Stepper Motor JK42HS34-0424A
                         http://scionelectronics.com/product/jk42hs34-0424a-nema-17-stepper-motor/
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   Motor:                12V NEMA 17 Stepper Motor JK42HS34-0424A
                         http://scionelectronics.com/product/jk42hs34-0424a-nema-17-stepper-motor/
   IMU:                  9-DOF IMU module
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Radio Transceiver:    2.4G Wireless Serial Transparent Transceiver Module for Arduino
                         http://www.dx.com/p/2-4g-wireless-serial-transparent-transceiver-module-for-arduino-441995#.WaMOUCiGPIU
   DRV8825               Step-Down Stepper Motor Driver Carrier Reprap 4-Layer PCB Set - Purple
                         http://www.dx.com/s/drv8825
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   5      4      3    2    (Arduino Pro Mini Pin)
   |      |      |    |
   DIR_R   STP2   DIR_L STP1 (DRV8825 pin - Motor 1 and 2)
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
   Datasheet Atmega328P: 8-bit AVR Microcontrollers:
   http://www.atmel.com/Images/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf
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


int dPause    =  100;

void setup() {

  pinMode(DIR_L, OUTPUT);
  pinMode(STEP_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(STEP_R, OUTPUT);

}

/* motor  1/4 throttle */

void loop()
{

  digitalWrite( DIR_L, HIGH );
  digitalWrite( DIR_R, LOW );
  stepGo();
}

void stepGo() {

  digitalWrite(STEP_L, HIGH);
  digitalWrite(STEP_R, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP_L, LOW);
  digitalWrite(STEP_R, LOW);
  delayMicroseconds(dPause);

}
