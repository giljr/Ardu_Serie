
//
//int Blue = 14;               // Firing order for 28BYJ-48 Stepper Motor
//int Pink = 15;              // Then yellow coil and so on...
//int Orange = 16;               // First Blue coil
//int Yellow = 10;
//
int Blue = 2;               // Firing order for 28BYJ-48 Stepper Motor
int Pink = 3;              // Then yellow coil and so on...
int Orange = 4;               // First Blue coil
int Yellow = 5;

long del = 500000; // 50.000 (+SPEED) -> 999.000 (-SPEED)// ABOVE this figure the motor will stall:/

void setup() {
  pinMode(Blue, OUTPUT);
  pinMode(Pink, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Orange, OUTPUT);


}

void phaseOne() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void phaseTwo() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, HIGH);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseThree() {
  digitalWrite(Blue, HIGH);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, HIGH);
  delayMicroseconds(del);
}
void phaseFour() {
  digitalWrite(Blue, HIGH);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Orange, LOW);
  delayMicroseconds(del);
}
void motorOff() {
  digitalWrite(Blue, LOW);
  digitalWrite(Pink, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Orange, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  for (int e = 0; e <= 5; e++) {
    //for (int i = 0; i <= 40; i++) {
    for (int i = 0; i <= 35; i++) {
      phaseOne();
      phaseTwo();
      phaseThree();
      phaseFour();
    }
    //for (int j = 0; j <= 40; j++) {
    for (int j = 0; j <= 35; j++) {
      phaseFour();
      phaseThree();
      phaseTwo();
      phaseOne();

    }
  }


}
