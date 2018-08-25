/* Project Ardu_Serie # 43
 * Working w/CTC Mode Part I - Atmega328p IC - Arduino MC
 * Ino File: _43_timer2_toggle_LED_01.ino
 * Use: TIMER2 
 *
 * Objective: We need to flash an LED every 100 ms (10 Hertz).
 * 
 * Instructions:
 * (1) We have a crystal of XTAL 16 MHz (Arduino);
 * (2) TIMER2 is 8-bit; Max Count is 225;
 * (3) Divide CPU Frequency though chosen prescaler (MAX PS = 1024): 
 *                                   16MHz/1024 = 15.625Hz
 * (4) Divide the Result though inverse of the desired milli-frequency (10Hz): 
 *                                   15.625/(1/10)*1000 = 156,25 (OCR2A = 156)
 * (5) Verify the Result against the max timer count value: 156 < 256 (Success)
 * (6) If fail, chose bigger prescaler (or lower the frequency);  That's all!
 *
 * Note: 
 * For use of the Christian Zeitnitz Soundcard Oscilloscope
 * (  https://www.zeitnitz.eu/scope_en   )  the working range is 10 - 15Hz; 
 * Perhaps using a professional oscillator this range would be much larger;
 * In any case here is the register of this experiment
 * with the electronics apparatus that I have on hand right now :-( 
 *
 * ---------------------------------------------------------------------------------
 *    Hz | Got Hz | Got ms| Inverse_Milli_Freq     | Calculations           | OCR2A
 * --------------------------------------------------------------------------------- 
 *    15 | 17,668 | 56,90 | (1/15)*1000= 66,7 ~ 70 | 16000000/1024/ 70= 223 | 220
 *    10 |  9,944 | 99,58 | (1/10)*1000= 100       | 16000000/1024/100= 156 | 156
 * ---------------------------------------------------------------------------------
 *
 * Created: 24/08/2018 18:18:09
 * Author : j3
 * WebPage: 
 * 
 * Based on the work of Mayank: AVR Timers – CTC Mode  
 * http://maxembedded.com/2011/07/avr-timers-ctc-mode/
 *  *
 * Terms of use:
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>

                            // initialize timer, interrupt and variable
void timer2_init()
{
                            // set up timer with prescaler = 1024 and CTC mode
  TCCR2A |= (1 << WGM21) | (0 << WGM20);
  TCCR2B |= (0 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);

                            // initialize counter
  TCNT2 = 0;

                            // initialize compare value for 10 Hz
  OCR2A = 220;
}

                            // initialize timer, interrupt and variable
void setup() {

                            // connect led to pin PB5 - INBUILD_LED - Arduino PIN 13
  DDRB |= (1 << PINB5);

                            // initialize timer2 (table above)
  timer2_init();


}

// loop forever
void loop() {

                            // check whether the flag bit is set
                            // if set, it means that there has been a compare match
                            // and the timer has been cleared
                            // use this opportunity to toggle the led
  if (TIFR2 & (1 << OCF2A))
  {
    PORTB ^= (1 << PINB5);  // toggles the led
  }

                            // wait! we are not done yet!
                            // clear the flag bit manually since there is no ISR to execute
                            // clear it by writing '1' to it (as per the datasheet)
  TIFR2 |= (1 << OCF2A);

                            // yeah, now we are done!
}

