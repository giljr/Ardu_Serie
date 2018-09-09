/*
   Project Ardu_Serie # 45
     Working w/ MultiFile Sketch - Part 0 - Atmega328p IC - Arduino MC
     This will be our starting code!
     This code tests the buttons and writes the time on for control
   
   Board: Arduino LCD KeyPad Shield
          https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
          
   Explanation: 
     Let's use the libraries provided by  http://www.dfrobot.com
     This libraries will facilitate our coding\o/
   
   Libraries:
     http://www.dfrobot.com/image/data/DFR0009/LCDKeypad.zip
     http://www.dfrobot.com/image/data/DFR0009/DFR_Key.zip
    
    ********************************************************

     This program will test the LCD panel and the buttons
     Mark Bramwell, July 2010

     ********************************************************
   Note:
     In order to adapt our board, we modified the files of these libraries;
     watch the video to better understand the changes if the bookstore 
     does not work on your board;)
   
   Web site:

   Terms of use:
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>



// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
//LM35
float tempC;
int reading;
int tempPin = 1;

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;
  */
  // For V1.0 comment the other threshold and use the one below:

  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}

void setup()
{
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
  lcd.print("Push the buttons"); // print a simple message
}

void loop()
{
  lcd.setCursor(9, 1);           // move cursor to second line "1" and 9 spaces over
  lcd.print(millis() / 1000);    // display seconds elapsed since power-up

  lcd.setCursor(0, 1);           // move to the begining of the second line
  lcd_key = read_LCD_buttons();  // read the buttons

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
      {
       lcd.print("RIGHT    ");
        break;
      }
    case btnLEFT:
      {
        lcd.print("LEFT   ");
        break;
      }
    case btnUP:
      {
        lcd.print("UP    ");
        break;
      }
    case btnDOWN:
      {
        lcd.print("DOWN  ");
        break;
      }
    case btnSELECT:
      {
        lcd.print("SELECT");
        break;
      }
    case btnNONE:
      {
        lcd.print("NONE  ");
        break;
      }
  }

}

