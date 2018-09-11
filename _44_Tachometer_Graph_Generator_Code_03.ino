/*
  Project name: Quick Fun Speed Lab 
    Simpler experiment -  Closed Loop Motion Control - Small Fan
    Flavour III - Graph Genearator code for tachometer (file size: ???? bytes)
    Hex File: _44_Tachometer_Graph_Generator_Code_03.ino
  Revision History:
    01/09/2018:
  Description:
    with the intention of collecting data to plot in a graph and understand 
    once and for all the code I another arduino will collect the data (via Sketch # 03) 
    probing PB3, PB5 & Fun Output using A0, A1 & A3 pins; 
    In other words: I modified the code I to have 3 types of output and then have data
    to be plotted in a graph for studying closed-loop motion-control; 
    see the video & web page for apprehension of this concept.


  MCU:             ATMega328p-pu       Arduino 1.8.5 - @16MHz       http://www.arduino.cc/
  Connections: Yellow wire to Arduino Pin D5, Gnd & VCC
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

*/


          // ATtiny85 outputs
#include <FlexiTimer2.h>           // use timer for sampling to get even time base for data

const int READ_A0_PIN = A0;
const int READ_A1_PIN = A1;
const int READ_A2_PIN = A2;



int inverted_value = 0;

void setup() {
  Serial.begin(2000000);            // initialize serial communications (to match that used by PlotNValues.pde)
  pinMode(READ_A0_PIN, INPUT);
  pinMode(READ_A1_PIN, INPUT);
  pinMode(READ_A2_PIN, INPUT);

  FlexiTimer2::set(10, sample);
  FlexiTimer2::start();
}

void loop() {
          //wait interruptions
}

          // sample values and pipe to processing
void sample() {
  String result = "";
  Serial.println(result + analogRead(READ_A0_PIN) + "," + analogRead(READ_A1_PIN)/2 + "," + analogRead(READ_A2_PIN)/2);
}
