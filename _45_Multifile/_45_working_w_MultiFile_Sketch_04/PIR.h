class PIR {

  private:

    int _ledPin;
    int _pirPin;
    int _pirState;


  public:

    void PIR_Setup(int ledPin, int pirPin, int pirState) {

      _ledPin = ledPin;
      _pirPin = pirPin;
      _pirState = pirState;

      pinMode(ledPin, OUTPUT);              //PIR Setup
      pinMode(pirPin, INPUT);

    }

    void PIR_Loop() {

      int pirValue = digitalRead(_pirPin);   //PIR Loop
      if (pirValue == HIGH) {               // check if the input is HIGH
        digitalWrite(_ledPin, HIGH);         // turn LED ON
        if (_pirState == LOW) {
          _pirState = HIGH;
        }
      } else {
        digitalWrite(_ledPin, LOW);          // turn LED OFF
        //lcd.print("Motion ended!   ");
        _pirState = LOW;

      }

    }

    void PIR_Display() {

      
        if (_pirState == HIGH) {

          lcd.print("Motion detected!");
          //break;
        };

        if (_pirState == LOW) {
          lcd.print("Motion ended!   ");
          //break;

        }

    }

};
PIR PIR;
