/* Project Ardu_Serie # 45
   Working w/ MultiFile Sketch - Part II - Atmega328p IC - Arduino MC

   Ino File: _45_working_w_MultiFile_Sketch_02.ino
   Use: 1602 LCD Board Keypad Shield

   Objective: Hi, Your code for the project is getting pretty long in the Arduino IDE?
   Is there a good way to split things up a bit using multiple files with headers and all that? 
   Yes! we're gonna publish [TODO: Number of Codes] codes from now on to show you how splitting codes…
   Here is the step-by-step process.

   Instructions:
   (1) Understand all the code in its monolitic fashion (Part I); where each sensor is configured and looped?
   (2) Transfer all the codes relative to the HC-SR04 to sheet HC-SR04.h (Part II);
   (3) Transfer all the codes relative to the PIR to sheet PIR.h (Part III);
   (4) Transfer all the codes relative to the TMP36 to sheet TMP36.h (Part IV);
       That's all! Now your ino file is clean and maintenance-free:)

   Connections:
   Board: 1602 LCD Board Keypad Shield Blue Backlight For Arduino
          https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)

    HC-SR04
    TRIG -> 11
    ECHO -> 12

    LCD
    D4 -> 4
    D5 -> 5
    D6 -> 6
    D7 -> 7

    PIR -> 2

    TMP36 -> A1

    LED -> 13

   Note:
    Not always the simulation in Proteus guarantees the correction of the code;
    we should always test our code on the real hardware;)

   Based on the work of Arduino Magix :
    How to Use Arduino Tabs and Code Spliting in Arduino IDE
    https://youtu.be/ylweRKU9VLE

   Created: Sept/2018
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

#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>             //Sample using https://www.dfrobot.com library

                                        // select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
                                        // initialise the keypad
DFR_LCD_Keypad keypad(A0, &lcd);

#include "HC_SR04.h"                    // Warning: this header should come after the lcd boot

                                        // define the key value used by the panel and buttons
int key;
                                        // TMP36 variables
float tempC;
int reading;
int tempPin = 1;
                                        //custom degree character
byte degree[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000,
};


int ledPin = 13;                        //PIR variables
int pirPin = 2;
int pirState = LOW;

void setup()
{
                                        //TMP36 - Read Note on case KEY_DOWN below;)
                                        //analogReference(INTERNAL);

                                        //LCD setup
  lcd.createChar(0, degree);
  lcd.begin(16, 2);                     // start the library
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Push the buttons");        // print a simple message

  pinMode(ledPin, OUTPUT);              //PIR Setup
  pinMode(pirPin, INPUT);

  HC.HC_Setup(11, 12);

}

void loop()
{
  int pirValue = digitalRead(pirPin);   //PIR Loop
  if (pirValue == HIGH) {               // check if the input is HIGH
    digitalWrite(ledPin, HIGH);         // turn LED ON
    if (pirState == LOW) {
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW);          // turn LED OFF
    //lcd.print("Motion ended!   ");
    pirState = LOW;

  }

  lcd.setCursor(0, 1);                  // move to the begining of the second line
  //lcd_key = read_LCD_buttons();       // read the buttons
  key = keypad.read_key();              // read the buttons 
                                        // print the key selection to the LCD
  switch (key)                          // depending on which button was pushed, we perform an action
  {

    case KEY_RIGHT:
      {
        if (pirState == HIGH) {

          lcd.print("Motion detected!");          
        };

        if (pirState == LOW) {
          lcd.print("Motion ended!   ");          
        }
        break;
      }

    case KEY_LEFT:
      {
        HC.HC_Loop();
        HC.HC_Display();
        break;
      }
      
    case KEY_DOWN:
      { 
        reading = analogRead(tempPin);
        lcd.setCursor(0, 1);                  // move to the begining of the second line                                            
        float voltage = reading * 5.0;        // converting that reading to voltage
        voltage /= 1024.0;
                                            // now print out the temperature
        tempC = (voltage - 0.5) * 100 ;      //converting from 10 mv per degree wit 500 mV offset
                                            //to degrees ((voltage - 500mV) times 100)
        lcd.print(tempC, 1);
        lcd.setCursor(4, 2);                  // move cursor to second line "1" and 4 spaces over
        lcd.write((byte)0);                   // display custom degree symbol
        lcd.print("C");
        delay(100);     
        break;
      }

    case KEY_SELECT:
      {
        lcd.print("Thank you!I'm J3");
        break;
      }

    case KEY_NONE:
      {
        lcd.print("                ");
        break;
      }
  }

}

