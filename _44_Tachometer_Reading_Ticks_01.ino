/*
  Project name: Quick Fun Speed Lab 
    Simpler experiment -  Closed Loop Motion Control - Small Fan
    Flavour I - tachometer: Code From the Book Arduino Internals (file size: ???? bytes)
    Hex File: _44_Tachometer_Reading_Ticks_01.ino
  Revision History:
    01/09/2018:
  Description:
    An 80mm cooling fan has a built-in brushless DC motor, a controller, and a tachometer
    output for measuring the fan speed, making it an excellent tool for 
    studying closed-loop motion-control.
designs.

  MCU:             ATMega328p-pu       Arduino 1.8.5 - @16MHz       http://www.arduino.cc/
  Connections:  Arduino Pin D5 -> Yellow wire; Pin D6 -> Gnd (Black wire); Pin D4 -> VCC (Red Wire)
    See Official Youtube channel video: [TODO: YOUTUBE LINK GOES HERE!!!]
    Web Page: [TODO: MEDIUM LINK GOES HERE!!!]

  Based on: Arduino Internals Book: https://www.amazon.com/Arduino-Internals-Technology-Action-Wheat/dp/1430238828
    Github code: https://github.com/Apress/arduino-internals/blob/master/978-1-4302-3882-9_CH13/13-12.ino

  ///////////////////////////////////////////////////////////////////////////////////////
  //Terms of use
  ///////////////////////////////////////////////////////////////////////////////////////
  //THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  //IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  //FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  //AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  //LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  //OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  //THE SOFTWARE.
  ///////////////////////////////////////////////////////////////////////////////////////

*/                                    // Fun's Connections definition
                                      // In case using bare Arduino: Connect the fun's terminal
                                      // on pins 5, 6 and 7 (SGN  to pin 5)
//#define VCC 6                       // VCC    - FUN'S  RED   WIRE - POWER SUPPLY BUS
#define SGN 5                         // SIGNAL - FUN'S YELLOW WIRE - TACHOMETER                             
//#define GND 7                       // VCC    - FUN'S  BLACK WIRE - POWER SUPPLY BUS

unsigned int fan_odometer = 0;        // count of fan tachometer ticks
volatile byte update_flag = 0;        // used to signal update of odometer reading

ISR(TIMER2_COMPA_vect) {
  static byte prescaler;
  
  if (prescaler) {
    prescaler--;
  } else {
    prescaler = 125;                  // reset prescaler
    fan_odometer = TCNT1;             // capture odometer reading
    update_flag = 1;                  // set flag
  }
  
}

void setup() {
  //pinMode(VCC, OUTPUT);             // VCC source current;   On Power bus  the speed increases up 740 RPM 
  //pinMode(GND, OUTPUT);             // GND ground reference; On Pin's set to VCC/GND the speed is 450 RPM 
  //digitalWrite(VCC, HIGH);          // enable pullup resistor for D5/PD5/T1 for tachometer input
  digitalWrite(SGN, HIGH);            // SIGNAL - tachometer
  //digitalWrite(GND, LOW);           // GND - Power Supply
  
                                      // timer/counter1 counts fan tachometer pulses 
                                      // on rising edge of T1, 2 per revolution
  TCCR1A = 0 << WGM11 | 0 << WGM10;
  TCCR1B = 0 << WGM13 | 0 << WGM12 | 1 << CS12 | 1 << CS11 | 1 << CS10;
  
                                      // timer/counter2 provides 125 interrupts per second
  TCCR2A = 1 << WGM21 | 0 << WGM20;
  TCCR2B = 0 << WGM22 | 1 << CS22 | 1 << CS21 | 1 << CS20;
  OCR2A = 124;                        // n-1
  TIMSK2 = 1 << OCIE2A;               // enable compare match interrupt
  
  Serial.begin(9600);
  Serial.println("Fan Speed in RPM");
}

void loop() {
  static unsigned int previous_fan_odometer = 0;
  
  while (update_flag == 0);           // wait for update to occur
  
  update_flag = 0;                    // reset update flag
  
  Serial.println(((fan_odometer - previous_fan_odometer) * 60) / 2); // in RPM
  
  previous_fan_odometer = fan_odometer;
}
