class TMP36 {

  private:

    // TMP36 variables
    float _tempC;
    int _reading;
    int _tempPin; // = 1;
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

  public:

    void TMP36_Setup(int tempPin) {
      int reading;
      int tempC;
      _tempPin = tempPin;
      _reading = reading;
      _tempC = tempC;


      //LCD setup
      lcd.createChar(0, degree);

    }

    void TMP36_Loop() {
      
      int reading;
      
      reading = analogRead(_tempPin);
      lcd.setCursor(0, 1);                  // move to the begining of the second line                                            
      float voltage = reading * 5.0;        // converting that reading to voltage
      voltage /= 1024.0;
                                            // now print out the temperature
      _tempC = (voltage - 0.5) * 100 ;      //converting from 10 mv per degree wit 500 mV offset
                                            //to degrees ((voltage - 500mV) times 100)
      
    }

    void TMP36_Display() {

      lcd.print(_tempC, 1);
      lcd.setCursor(4, 2);                  // move cursor to second line "1" and 4 spaces over
      lcd.write((byte)0);                   // display custom degree symbol
      lcd.print("C");
      delay(100);

    }

};
TMP36 TMP36;
