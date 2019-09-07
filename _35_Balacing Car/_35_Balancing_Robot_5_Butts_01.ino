#define DIR 2
#define STEP 3
#define BUTTON A0

#define M0 4
#define M1 5
#define M2 6



int SELECT[]  =  {0, 20};    // pulsante A
int LEFT[]    =  {100, 200}; // pulsante B
int RIGTH[]   =  {300, 390}; // pulsante C
int UP[]      =  {400, 500}; // pulsante D
int DOWN[]    =  {700, 830}; // pulsante E

int dPause    =  100;
int dirLevel  = HIGH;

/**********************************************************/

void setup() {
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  pinMode( BUTTON, INPUT );

  Serial.begin( 9600 );
  Serial.println( "Test" );
}

/**********************************************************/

void loop()
{
  int buttonPress = ctrlButton( analogRead( BUTTON ) );
  //Serial.println(analogRead( BUTTON ) );
  //Serial.println(buttonPress);
  //delay(100); 

  if ( buttonPress == 1 ) {
    // 1/2 Step
    dPause = 50;
    digitalWrite(M0, HIGH );
    digitalWrite(M1, LOW  );
    digitalWrite(M2, LOW  );
  }

  if ( buttonPress == 2 ) {
    // 1/4 Step
    dPause = 25;
    digitalWrite(M0, LOW  );
    digitalWrite(M1, HIGH );
    digitalWrite(M2, LOW  );
  }

  if ( buttonPress == 3 ) {
    if ( dirLevel == HIGH ) dirLevel = LOW;
    else                    dirLevel = HIGH;
  }

  if ( buttonPress == 4 ) {
    digitalWrite(M0, LOW  );
    digitalWrite(M1, LOW  );
    digitalWrite(M2, LOW
                );
  }

  if ( buttonPress == 5 ) {
    return;
  }

  digitalWrite( DIR, dirLevel );
  stepGo();
}

/**********************************************************/

int ctrlButton( int button ) {

  if ( SELECT[0] <= button && button <= SELECT[1] )  {
    return 1;
  }
  if ( LEFT[0] <= button && button <= LEFT[1] )      {
    return 2;
  }
  if ( RIGTH[0] <= button && button <= RIGTH[1] )    {
    return 3;
  }
  if ( UP[0] <= button && button <= UP[1] )          {
    return 4;
  }
  if ( DOWN[0] <= button && button <= DOWN[1] )      {
    return 5;
  }

  return 0;

}

/**********************************************************/

void stepGo() {
 Serial.println( dPause );

  digitalWrite(STEP, HIGH);
  delayMicroseconds(dPause);
  digitalWrite(STEP, LOW);
  delayMicroseconds(dPause);
}
