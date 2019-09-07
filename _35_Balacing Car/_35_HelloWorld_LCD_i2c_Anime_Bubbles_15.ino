/* JayThree Balancing Car Project — Part 4/5
 * J3BC Circuit Board Prototyping Tips and Tricks (with Slides)
 * https://medium.com/p/4964ab5af128/edit
 * 
 * Description: animated bubbles appear on the LCD!
 * 
 * Notes for this test:
 * 
 * I first tested the code on Arduino ONE;
 * My Arduino IDE is installed manually in the C directory → C: \ arduino;
 * Then I downloaded the zip of the library by Francisco Malpartida New LiquidCrystal 
 * on https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/;
 * Unzip and extract it to the directory C:\ arduino \ libraries \ NewliquidCrystal;
 * I uplosded this code for Arduino Pro mini code: _35_J3_GY85_IMU_15.ino;
 * Do not forget to power the LCD with 5v. Otherwise nothing will appear on the screen!
 * */
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//Include LCD and I2C library
//#include <LCD.h>



//#define BACKLIGHT_PIN     13

//LiquidCrystal_I2C lcd(0x38);  // Set the LCD I2C address
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
//LiquidCrystal_I2C lcd(0x38, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address


// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
   
};

void setup()
{
   int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
  lcd.begin(16, 2);                            // Initialize the LCD
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);                      // Activate backlight
  lcd.clear();  
  // Switch on the backlight
  //pinMode ( BACKLIGHT_PIN, OUTPUT );
  //digitalWrite ( BACKLIGHT_PIN, HIGH );
  
                               // Clear the LCD

   for ( int i = 0; i < charBitmapSize; i++ )
   {
      lcd.createChar ( i, (uint8_t *)charBitmap[i] );
   }

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" FORUM - fm   ");
  delay ( 1000 );
}

void loop()
{
   lcd.home ();
   // Do a little animation by writing to the same location
   for ( int i = 0; i < 2; i++ )
   {
      for ( int j = 0; j < 16; j++ )
      {
         lcd.print (char(random(7)));
      }
      lcd.setCursor ( 0, 1 );
   }
   delay (200);
}
