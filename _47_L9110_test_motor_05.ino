/* Project Ardu_Serie # 47
  J3C3 - Controlling Motor - Techniques For Driving Inductive Load  - L9110S
   Code V
   Ino File: _47_L9110_test_motor_05.ino
   Use: L9110S - https://www.elecrow.com/download/datasheet-l9110.pdf

   Objective: Hi, What are the best options for controlling motors with inductive loads?
   Here I invastigate L9110S - Motor control driver chip;)

   Instructions (CODE SEQUENCE)
      (1) The control: simply the DC MOTOR, L9110 and Arduino;
      (2) We add the movement forward and back frozen in the code;
      (2) We add a potentiometer to vary the speed; no L9110 chip; single direction;
      (4) We add the potentiometer and the state machine; L9110 chip returns:)
      (5) We refined the code to present a menu, a la carte!; That's all!

   Connections:
    LM9110 & MOTOR
    A -> 3
    B -> 4
    vcc in vin & gnd (use external power 9v)
    POT
    TRIG -> A0
    vcc & gnd     
    
   Related Banana Robotics items:   
    BR010038 HG7881 (L9110) Dual Channel Motor Driver Module
    https://www.BananaRobotics.com/shop/HG7881-(L9110)-Dual-Channel-Motor-Driver-Module
    https://www.BananaRobotics.com

   Note:
    Whenever possible add complexity gradually to your code:)

   Created: Sept/2018
   Author : J3
   WebPage:

   Base on: Control de Motores - Que puente en H usar? by J.Aguirre
            https://youtu.be/rXAxxvEy-fc

   Terms of use:
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
/*
  HG7881_Motor_Driver_Example - Arduino sketch
   
  This example shows how to drive a motor with using HG7881 (L9110) Dual
  Channel Motor Driver Module.  For simplicity, this example shows how to
  drive a single motor.  Both channels work the same way.
   
  This example is meant to illustrate how to operate the motor driver
  and is not intended to be elegant, efficient or useful.  
*/
 
                         // wired connections
#define AIA 3            // D3 --> Motor B Input A --> MOTOR B +
#define AIB 4            // D4 --> Motor B Input B --> MOTOR B -
 
// functional connections
#define MOTOR_B_PWM AIA  // Motor B PWM Speed
#define MOTOR_B_DIR AIB  // Motor B Direction
 
                         // the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 50      // arbitrary slow speed PWM duty cycle
#define PWM_FAST 200     // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000   // brief delay for abrupt motor changes
 
void setup()
{
  Serial.begin( 9600 );
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
}
 
void loop()
{
  boolean isValidInput;
  // draw a menu on the serial port
  Serial.println( "-----------------------------" );
  Serial.println( "MENU:" );
  Serial.println( "1) Fast forward" );
  Serial.println( "2) Forward" );
  Serial.println( "3) Soft stop (coast)" );
  Serial.println( "4) Reverse" );
  Serial.println( "5) Fast reverse" );
  Serial.println( "6) Hard stop (brake)" );
  Serial.println( "-----------------------------" );
  do
  {
    byte c;
                         // get the next character from the serial port
    Serial.print( "?" );
    while( !Serial.available() )
      ;                  // LOOP...
    c = Serial.read();
                         // execute the menu option based on the character recieved
    switch( c )
    {
      case '1':          // 1) Fast forward
        Serial.println( "Fast forward..." );
        // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
                         // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
        analogWrite( MOTOR_B_PWM, 255-PWM_FAST ); // PWM speed = fast
        isValidInput = true;
        break;      
         
      case '2':          // 2) Forward      
        Serial.println( "Forward..." );
                         // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
                         // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, HIGH );        // direction = forward
        analogWrite( MOTOR_B_PWM, 255-PWM_SLOW ); // PWM speed = slow
        isValidInput = true;
        break;      
         
      case '3':                                   // 3) Soft stop (preferred)
        Serial.println( "Soft stop (coast)..." );
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        isValidInput = true;
        break;      
 
      case '4':          // 4) Reverse
        Serial.println( "Fast forward..." );
                         // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
                         // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, LOW );         // direction = reverse
        analogWrite( MOTOR_B_PWM, PWM_SLOW );     // PWM speed = slow
        isValidInput = true;
        break;      
         
      case '5':          // 5) Fast reverse
        Serial.println( "Fast forward..." );
                         // always stop motors briefly before abrupt changes
        digitalWrite( MOTOR_B_DIR, LOW );
        digitalWrite( MOTOR_B_PWM, LOW );
        delay( DIR_DELAY );
                         // set the motor speed and direction
        digitalWrite( MOTOR_B_DIR, LOW ); // direction = reverse      
        analogWrite( MOTOR_B_PWM, PWM_FAST ); // PWM speed = fast
        isValidInput = true;
        break;
         
      case '6':          // 6) Hard stop (use with caution)
        Serial.println( "Hard stop (brake)..." );
        digitalWrite( MOTOR_B_DIR, HIGH );
        digitalWrite( MOTOR_B_PWM, HIGH );
        isValidInput = true;
        break;      
         
      default:
                         // wrong character! display the menu again!
        isValidInput = false;
        break;
    }
  } while( isValidInput == true );
 
                         // repeat the main loop and redraw the menu...
}

