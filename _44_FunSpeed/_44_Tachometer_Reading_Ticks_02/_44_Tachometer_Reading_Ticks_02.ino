/*
  Project name: Quick Fun Speed Lab 
    Simpler experiment -  Closed Loop Motion Control - Small Fan
    Flavour II - tachometer: Probing Arduino Pins PB3 & PB5 (file size: ???? bytes)
    Hex File: _44_Tachometer_Reading_Ticks_02.ino
  Revision History:
    01/09/2018:
  Description:
    With the intention of collecting data to plot in a graph and understand 
    once and for all the code I another arduino will collect the data (via Sketch # 03) 
    probing PB3, PB5 & Fun Output using A0, A1 & A3 pins; 
    In other words: I modified the code I to have 3 types of output and then have data
    to be plotted in a graph for studying closed-loop motion-control; 
    see the video & web page for apprehension of this concept.

  MCU:             ATMega328p-pu       Arduino 1.8.5 - @16MHz       http://www.arduino.cc/
  Connections: Yellow wire to Arduino Pin D5, Gnd & VCC
     See Official youtube channel video: [TODO: YOUTUBE LINK GOES HERE!!!]
     Web Page: [TODO: MEDIUM LINK GOES HERE!!!]
  
  Datasheet Atmega328P:
  http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf

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

*/                             // Fun's Connections definition
#define VCC 6                  // VCC    - FUN'S  RED   WIRE - POWER SUPPLY BUS
#define SGN 5                  // SIGNAL - FUN'S YELLOW WIRE - TACHOMETER                             
#define GND 7                  // GND    - FUN'S  BLACK WIRE - POWER SUPPLY BUS

unsigned int fan_odometer = 0; // count of fan tachometer ticks
volatile byte update_flag = 0; // used to signal update of odometer reading

ISR(TIMER2_COMPA_vect) {
  static byte prescaler;
  
  if (prescaler) {
    prescaler--;
  } else {
    prescaler = 125; // reset prescaler
    fan_odometer = TCNT1; // capture odometer reading
    update_flag = 1; // set flag
  }
  
}

void setup() {
  pinMode(VCC, OUTPUT);        // VCC source current;   On Power bus  the speed increases up 740 RPM 
  pinMode(GND, OUTPUT);        // GND ground reference; On Pin's set to VCC/GND the speed is 450 RPM 
  digitalWrite(VCC, HIGH);     // enable pullup resistor for D5/PD5/T1 for tachometer input
  digitalWrite(SGN, HIGH);     // SIGNAL - tachometer
  digitalWrite(GND, LOW);      // GND - Power Supply
  digitalWrite(SGN, HIGH);       // enable pullup resistor for D5/PD5/T1 for tachometer input
  
          // timer/counter1 counts fan tachometer pulses on rising edge of T1, 2 per revolution
          // 1ª Modification: Datasheet page 174
          // to make OC1A pin  - Arduino PIN D9 (PB1) - as output at each interrupt insert, 
          // on TCCR1A, 1 << COM1A1 & 1 <<COM1A0 
  TCCR1A = 1 << COM1A1 | 1 <<COM1A0 | 0 << WGM11 | 0 << WGM10;
  TCCR1B = 0 << WGM13 | 0 << WGM12 | 1 << CS12 | 1 << CS11 | 1 << CS10;
  
          // timer/counter2 provides 125 interrupts per second
  TCCR2A = 1 << WGM21 | 0 << WGM20;
  TCCR2B = 0 << WGM22 | 1 << CS22 | 1 << CS21 | 1 << CS20;
  OCR2A = 124; // n-1
  TIMSK2 = 1 << OCIE2A;     // enable compare match interrupt
  
  Serial.begin(9600);
  Serial.println("Fan Speed in RPM");
}

void loop() {
  static unsigned int previous_fan_odometer = 0;
  
  while (update_flag == 0); // wait for update to occur
  
  update_flag = 0;          // reset update flag
            // 2ª Modification: Datasheet page 204
          // to make Arduino PB7 as output at each 125º interrupt 
  digitalWrite(8, HIGH);
  Serial.println(((fan_odometer - previous_fan_odometer) * 60) / 2); // in RPM
  
  previous_fan_odometer = fan_odometer;
}
