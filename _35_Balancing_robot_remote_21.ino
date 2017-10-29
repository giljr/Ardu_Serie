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
#include <Wire.h>                                                   //Include the Wire.h library so we can communicate with the Nunchuck

int nunchuk_address = 0x52;                                         //Nunchuk I2C address (0x52)
byte received_data[6], send_byte;                                   //Declare some global byte variables

void setup(){
  Serial.begin(9600);                                               //Start the serial port at 9600 kbps
  Wire.begin();                                                     //Start the I2C as master
  TWBR = 12;                                                        //Set the I2C clock speed to 400kHz
  Wire.begin();                                                     //Start the I2C bus as master
  delay(20);                                                        //Short delay
  Wire.beginTransmission(nunchuk_address);                          //Start communication with the Nunchuck
  Wire.write(0xF0);                                                 //We want to write to register (F0 hex)
  Wire.write(0x55);                                                 //Set the register bits as 01010101
  Wire.endTransmission();                                           //End the transmission
  delay(20);                                                        //Short delay
  Wire.beginTransmission(nunchuk_address);                          //Start communication with the Nunchuck
  Wire.write(0xFB);                                                 //We want to write to register (FB hex)
  Wire.write(0x00);                                                 //Set the register bits as 00000000
  Wire.endTransmission();                                           //End the transmission
  delay(20);                                                        //Short delay
}

void loop(){
  Wire.beginTransmission(nunchuk_address);                          //Start communication with the Nunchuck.
  Wire.write(0x00);                                                 //We want to start reading at register (00 hex)
  Wire.endTransmission();                                           //End the transmission
  Wire.requestFrom(nunchuk_address,6);                              //Request 6 bytes from the Nunchuck
  for(byte i = 0; i < 6; i++) received_data[i] = Wire.read();       //Copy the bytes to the received_data array
  send_byte = B00000000;                                            //Set the send_byte variable to 0
  if(received_data[0] < 80)send_byte |= B00000001;                  //If the variable received_data[0] is smaller then 80 set bit 0 of the send byte variable
  if(received_data[0] > 170)send_byte |= B00000010;                 //If the variable received_data[0] is larger then 170 set bit 1 of the send byte variable
  if(received_data[1] < 80)send_byte |= B00001000;                  //If the variable received_data[1] is smaller then 80 set bit 3 of the send byte variable
  if(received_data[1] > 170)send_byte |= B00000100;                 //If the variable received_data[1] is larger then 170 set bit 2 of the send byte variable
  if(send_byte)Serial.print((char)send_byte);                       //Send the send_byte variable if it's value is larger then 0
  delay(40);                                                        //Create a 40 millisecond loop delay
}



