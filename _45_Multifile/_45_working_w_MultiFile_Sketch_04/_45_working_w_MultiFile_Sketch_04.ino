/* Project Ardu_Serie # 45
   Working w/ MultiFile Sketch - Part IV - Atmega328p IC - Arduino MC

   Ino File: _45_working_w_MultiFile_Sketch_04.ino
   Use: 1602 LCD Board Keypad Shield

   Objective: Hi, Your code for the project is getting pretty long in the Arduino IDE?
   Is there a good way to split things up a bit using multiple files with headers and all that? 
   Yes! we're gonna publish [TODO: Number of Codes] codes from now on to show you how splitting codes…
   Here is the step-by-step process.

   Instructions:
   (1) Understand all the code in its monolitic fashion (Part I); where each sensor is configured and looped!
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
   WebPage: https://medium.com/jungletronics/arduino-multi-file-sketch-583a7833162c

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

#include "HC_SR04.h"                    // Warning: this headers should come after the lcd boot
#include "PIR.h"
#include "TMP36.h"

                                        // define the key used by the panel and buttons
int key;

void setup()
{


  lcd.begin(16, 2);                     // start the library
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Push the buttons");        // print a simple message

  HC.HC_Setup(11, 12);
  PIR.PIR_Setup(13, 2, HIGH);
  TMP36.TMP36_Setup(1);
  
}

void loop()
{
  PIR.PIR_Loop();
  
  lcd.setCursor(0, 1);                  // move to the begining of the second line
  key = keypad.read_key();              // read the buttons
                                        // print the key selection to the LCD
  switch (key)                          // depending on which button was pushed, we perform an action
  {

    case KEY_RIGHT:
      {
       PIR.PIR_Display();
       
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
                
        TMP36.TMP36_Loop();
        TMP36.TMP36_Display();
                
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

