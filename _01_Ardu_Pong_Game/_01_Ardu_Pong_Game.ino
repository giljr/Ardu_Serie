
/*
 *Project name:
     ARDUINO IN HIGH SERIE 01 - PONG GAME W/ 74HC138N!!!
     ino File: _01_Ardu_Pong_Game.ino
  *Revision History:
     20160228:
       - found on internet (gilj3)
       http://wiki.edwindertien.nl/doku.php?id=modules:ledmatrix#schematic
 *Description:
     Let's play ping pong with Arduino? 
     We will use an counter, 74HC138N & 
     a cool code!!!Enjoy good & have fun!!!
 *MCU:              Arduino - @16MHz           http://www.arduino.cc/
 *MATRIZ Board:     Matriz HS2088BS  http://www.soldafria.com.br/matriz-de-leds-vermelho-anodo-8x8-hs-2088bs-hs-23088-p-3638.html
 *Connections:
     See: you.tube Video : https://youtu.be/ydJrrRyvgU8
  Notes: 
  * In a timer interrupt (timer2, 1kHz) the display routine is 
     called, outputting the global imageMem buffer. 
  * Autoplay can be defined for a static demo.
  */
  
#define AUTOPLAY true
unsigned char imageMem[8]={ // display buffer
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
float speed_x = 1;  // ball variables
float speed_y = 0.5;
float pos_x = 0;
float pos_y = 4;
 
float paddle_l,paddle_r;// paddle positions
int n;                  // display row counter
 
void setup(){
  for (int z=2; z<14; z++){
    pinMode(z,OUTPUT); // all pins output for driving the LED matrix
  }
  // intitialize interrupt timer 2 
  TCCR2A = 0; // no ocr or pwm
  TCCR2B = 4; //1<<CS22 | 0<<CS21 | 0<<CS20 -- for clock div 64
  TIMSK2 = 1<<TOIE2; // enable timer 1 interrupt 
  TCNT2 = 0x06; // 1 kHz
}
 
void loop(){
  float paddle = analogRead(0)/116;     // use one paddle variable
  if (AUTOPLAY){
    if (speed_x==-1) paddle_l = pos_y;   // autoplay
    else paddle_r = pos_y;
  }
  else {
    if (speed_x==-1) paddle_l = paddle;   // swap who's controlling the paddle
    else paddle_r = paddle;
  }
  if ((pos_x>6) && (speed_x==1)&& (pos_y<(paddle_r+1)) && (pos_y>(paddle_r-1))) {
    // reverse direction when hitting the bat
    speed_x=-1;
  }
  if ((pos_x<1) && (speed_x==-1) && (pos_y<(paddle_l+1)) && (pos_y>(paddle_l-1))){
    // reverse direction when hitting the bat
    speed_x=1;
  }
  pos_x += speed_x;
  pos_y += speed_y;
  if(pos_y >6) { // ceiling
    speed_y = speed_y * -1;
  }
  if(pos_y <1) { // bottom
    speed_y = speed_y * -1;
  }
  if(pos_x<0){ // ball is out left: right can serve
    pos_x=6;
    pos_y=paddle_r;
    speed_x=-1;
    speed_y = (float)random(0,10)/10;
  }
  if(pos_x>7) {// ball is out right: left can serve
    pos_x=1;
    pos_y=paddle_l;
    speed_x=1;
    speed_y = (float)random(0,10)/10;
  }
  clearScreen();
  drawBall((int)pos_x,(int)pos_y);
  drawBats((int)paddle_l,(int)paddle_r);
  delay(100);
}
void drawBats(int l, int r){
  imageMem[7] = (3<<l)>>1;
  imageMem[0] = (3<<r)>>1;
}
void drawBall(int x, int y){
  imageMem[7-x]=1<<y;               // draw the ball
}
void clearScreen(){
  for (int z=0;z<8;z++){
    imageMem[z]=0x00;
  }
}
ISR(TIMER2_OVF_vect) {
  TCNT2 = 0x06; // reload timer at 1kHz
  PORTD &= 0xE3; // clear PIN 2,3,4
  PORTD |= (n<<2)&0x1C; // set collumn trough the mux
  PORTD &=0x1F; // clear PIN 5,6,7
  PORTD |= (imageMem[n]<<5)&0xE0; // set row data on 5,6,7
  PORTB &=0xE0; // clear PIN 8,9,10,11,12
  PORTB |= (imageMem[n]>>3)&0x1F; // set row data on 8-12
  n++; // increase collumn counter
  if(n>7) {n=0;} // next image. Refreshrate = 1000/8 images/sec
}
