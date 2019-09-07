class HC_SR04 {

  private:

    int _trigPin;                       // HC-04 variables
    int _echoPin;
    long _duration;
    int _distanceCm;


  public:

    void HC_Setup(int trigPin, int echoPin) {

      long duration;                  // HC-04 variables
      int distance;

      _trigPin = trigPin;
      _echoPin = echoPin;
      _duration = duration;
      _distanceCm = distance;

                                     //HC-SR04 setup
      pinMode(_trigPin, OUTPUT);
      pinMode(_echoPin , INPUT);

    }

    void HC_Loop() {

                                      // Clears the trigPin
      digitalWrite(_trigPin, LOW);     //HC-SR04 Loop
      delayMicroseconds(2);           // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trigPin, LOW);
                                      // Reads the echoPin, returns the sound wave travel time in microseconds
      _duration = pulseIn(_echoPin, HIGH);
                                      // Calculating the distance
      _distanceCm = _duration * 0.034 / 2;
      lcd.print(_distanceCm);
      lcd.print(" Cm distance");


    }

    void HC_Display() {
      lcd.print("                ");
      lcd.print("UP   ");

    }

};
HC_SR04 HC;
