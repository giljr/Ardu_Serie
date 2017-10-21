/*
   Project name: JayThree Time_Lapse CAM Project #arduSerie 36
     A very cool project that captures images for a predetermined time. 
     Serves as a time lapse camera. It is widely used in biology, 
     for the study of human or animal behavior. Have a good time!!!
     
     Flavour VI - Working Code For J3 Time Lapse Camera v 1.0  
     INO File: _36_Grove_Serial_TimeLapseCAM_06.ino
   
   Revision History:
     Oct, 2017
       - v 1.0 - Medium webpage:
   Description:
            This final code takes picture at approximate 16s + 5s between pauses. It is automatic 
            and when it is in operation the green light flashes. If there is a problem, the red light
            flashes, warning the user of hardware problems. As soon as the camera starts to operate, 
            the green light flashes 5 times quickly indicating that everything is OK, 
            the RTC and SDCard are working. Thanks for the visit!
   MCU:                  Arduino Pro Micro - Atmega328 16MHz 5V @16MHz - https://www.sparkfun.com/products/12640
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   SDCard:               SD Card Module Slot Socket Reader LC Studio 3.3V/5V - https://goo.gl/gCUnEU
   DS1307 RTC:           DS1307 Real Time Clock breakout board - https://www.adafruit.com/product/264
   Serial Camera:        Grove - Serial Camera Kit - https://www.seeedstudio.com/Grove-Serial-Camera-Kit-p-1608.html?cPath=25_33                            
   Connections:          See Official youtube channel vids: https://goo.gl/unfHZA
   Based on:             http://jungletronics.blogspot.com.br/2014/06/blog-post.html
   Credit:               JR TimeLapse Cam Project!                  
   Cam Datasheet (pdf):  https://goo.gl/oGBQbx  (CJ-OV528 Protocol（通信协议） (2))
   Terms of use:  
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
     THE SOFTWARE.  
  //original notes:
   Date: 08/06/2014  Version: 0.1
  Date: 21/06/2014  Version: 0.3 (lights and reset buttons)
*/
//cam settings:
#include <arduino.h>
#define PIC_PKT_LEN    128                //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     Serial1
#define MON_SERIAL     Serial
#define PIC_FMT        PIC_FMT_VGA
#include <SPI.h>

#include <SD.h>   //For talking to SD Card
#include <Wire.h>  //For RTC
#include "RTClib.h" //For RTC

const byte cameraAddr = (CAM_ADDR << 5);  // addr
const int buttonPin = A6;                 // the number of the pushbutton pin
unsigned long picTotalLen = 0;            // picture length
int picNameNum = 0;
String picNameStr;
char picName[25];
//Define pins
//SD card is on standard SPI pins
//RTC is on Standard I2C Pins
const int CS_PIN      = 10;
const int SD_POW_PIN  = 8;
const int RTC_POW_PIN = A3;
const int RTC_GND_PIN = A2;
//Default rate of 5 seconds
long refresh_rate = 500000;
//Define an RTC object
RTC_DS1307 RTC;
//Initialize strings
String year, month, day, hour, minute, second;//, time, date;
//File myFile;
File logFile; // file pointer
DateTime nowTime; // for real time clock readings
unsigned long prev_time = 0; //timer for taking photos
String date = "";
String time = "";
String occasion = "";

const int REDLIGHT = 8; // controls the signal light indicating problem
const int GREENLIGHT = 5;  // controls the signal light indicating everything works fine
byte problem = 0; // to indicate trouble to the user
/*********************************************************************/
void setup()
{
  pinMode(REDLIGHT, OUTPUT);
  pinMode(GREENLIGHT, OUTPUT);

  Serial.begin(9600);
  //for cam
  Serial1.begin(9600);
  //pinMode(buttonPin, INPUT);    // initialize the pushbutton pin as an input


  Serial.println(F("Initializing Card"));

  //CS pin, and pwr/gnd pins are outputs
  pinMode(CS_PIN,   OUTPUT);
  pinMode(SD_POW_PIN, OUTPUT);
  pinMode(RTC_POW_PIN, OUTPUT);
  pinMode(RTC_GND_PIN, OUTPUT);

  //Setup power and ground pins for both modules
  //digitalWrite(SD_POW_PIN, HIGH);
  digitalWrite(RTC_POW_PIN, HIGH);
  digitalWrite(RTC_GND_PIN, LOW);

  //Initiate the I2C bus and the RTC library
  Wire.begin();
  RTC.begin();

  //If RTC is not running, set it to the computer's compile time
  if (! RTC.isrunning())
  {
    problem = 1;
    Serial.println(F("RTC is NOT running!"));
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  //Initialize SD card
  if (!SD.begin(CS_PIN))
  {
    problem = 1;
    Serial.println(F("Card Failure"));
    //Serial.println("Card Failure");
    return;
  }
  Serial.println(F("Card Ready"));


  ////Reading config file - TODO"!
  //Serial.println("Card Ready");
  //Read the configuration information (speed.txt)
  //  File commandFile = SD.open("speed.txt");
  //  if (commandFile)
  //  {
  //    Serial.println(F("Reading Command File"));
  //    //Serial.println("Reading Command File");
  //
  //    while(commandFile.available())
  //    {
  //      refresh_rate = commandFile.parseInt();
  //    }
  //    Serial.print(F("Refresh Rate = "));
  //    //Serial.print("Refresh Rate = ");
  //    Serial.print(refresh_rate);
  //    Serial.println(F("ms"));
  //    commandFile.close();
  //  }
  //  else
  //  {
  //    Serial.println(F("Could not read command file."));
  //    return;
  //  }
  //cam init
  initialize();

}
/*********************************************************************/
void loop()
{
  if (problem) { // If something is wrong, do nothing but alert the user
    sos(5, 50);
    delay(300);
  }
  picNameStr = date + time + occasion;
  int n = 0;
  while (1) {
    if (millis() >= prev_time + 6000) {
      Serial.println("\r\nbegin to take picture");
      delay(200);
      if (n == 0) preCapture();
      Capture();
      GetData();
      Serial.print("\r\nTaking pictures success ,number : ");
      Serial.println(n);
      n++ ;
      prev_time = millis();
    }
  }
}
/*********************************************************************/
void clearRxBuf()
{
  while (Serial1.available())
  {
    Serial1.read();
  }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len)
{
  for (char i = 0; i < cmd_len; i++) Serial1.print(cmd[i]);
}
/*********************************************************************/
void initialize()
{
  char cmd[] = {
    0xaa, 0x0d | cameraAddr, 0x00, 0x00, 0x00, 0x00
  }
  ;
  unsigned char resp[6];

  Serial1.setTimeout(500);
  while (1)
  {
    //clearRxBuf();
    sendCmd(cmd, 6);
    if (Serial1.readBytes((char *)resp, 6) != 6)
    {
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0)
    {
      if (Serial1.readBytes((char *)resp, 6) != 6) continue;
      if (resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break;
    }
  }
  cmd[1] = 0x0e | cameraAddr;
  cmd[2] = 0x0d;
  sendCmd(cmd, 6);
  Serial.println("\nCamera initialization done.");
  flashLight(5, 50); // we've got this far okay, first flashes are distinctive to mark beginning
  delay(300);
}
/*********************************************************************/
void preCapture()
{
  char cmd[] = {
    0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT
  };
  unsigned char resp[6];

  Serial1.setTimeout(100);
  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (Serial1.readBytes((char *)resp, 6) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break;
  }
}
/*********************************************************************/
void Capture()
{
  char cmd[] = {
    0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN >> 8) & 0xff , 0
  };
  unsigned char resp[6];

  Serial1.setTimeout(100);
  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (Serial1.readBytes((char *)resp, 6) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x06 && resp[4] == 0 && resp[5] == 0) break;
  }
  cmd[1] = 0x05 | cameraAddr;
  cmd[2] = 0;
  cmd[3] = 0;
  cmd[4] = 0;
  cmd[5] = 0;
  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (Serial1.readBytes((char *)resp, 6) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) break;
  }
  cmd[1] = 0x04 | cameraAddr;
  cmd[2] = 0x1;
  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (Serial1.readBytes((char *)resp, 6) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0)
    {
      Serial1.setTimeout(1000);
      if (Serial1.readBytes((char *)resp, 6) != 6)
      {
        continue;
      }
      if (resp[0] == 0xaa && resp[1] == (0x0a | cameraAddr) && resp[2] == 0x01)
      {
        picTotalLen = (resp[3]) | (resp[4] << 8) | (resp[5] << 16);
        Serial.print("picTotalLen:");
        Serial.println(picTotalLen);
        break;
      }
    }
  }

}
/*********************************************************************/
void GetData()
{
  unsigned int pktCnt = (picTotalLen) / (PIC_PKT_LEN - 6);
  if ((picTotalLen % (PIC_PKT_LEN - 6)) != 0) pktCnt += 1;

  char cmd[] = {
    0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00
  };
  unsigned char pkt[PIC_PKT_LEN];
  newfile();
  // Did the file open successfully?
  if (SD.exists(picName))
  {
    SD.remove(picName);
  }

  // Or something long enough to hold the longest file name you will ever use.
  if (!logFile) {
    sos(5, 50);
    Serial.println("logFile open fail...");
  }
  else {
    Serial1.setTimeout(1000);
    for (unsigned int i = 0; i < pktCnt; i++)
    {
      cmd[4] = i & 0xff;
      cmd[5] = (i >> 8) & 0xff;

      int retry_cnt = 0;
retry:
      delay(10);
      clearRxBuf();
      sendCmd(cmd, 6);
      uint16_t cnt = Serial1.readBytes((char *)pkt, PIC_PKT_LEN);

      unsigned char sum = 0;
      for (int y = 0; y < cnt - 2; y++)
      {
        sum += pkt[y];
      }
      if (sum != pkt[cnt - 2])
      {
        if (++retry_cnt < 100) goto retry;
        else break;
      }

      logFile.write((const uint8_t *)&pkt[4], cnt - 6);
      //flashLight(1, 50);
      //if (cnt != PIC_PKT_LEN) break;
    }
    cmd[4] = 0xf0;
    cmd[5] = 0xf0;
    sendCmd(cmd, 6);
  }
  logFile.close();
  flashLight(1, 50);
  picNameNum ++;
}
/*********************************************************************/
// Create new file on SD card
File newfile() {
  nowTime = timestamp(); // get the time, which we'll use for the filename

  String filename = String();
  //  if (nowTime.month() < 10) { // for goodness sake do a proper leading zero function that returns value!
  //    filename+= "0";
  //  }
  //  filename += nowTime.month();
  if (nowTime.day() < 10) {
    filename += "0";
  }
  filename += nowTime.day();
  if (nowTime.hour() < 10) {
    filename += "0";
  }
  filename += nowTime.hour();
  if (nowTime.minute() < 10) {
    filename += "0";
  }
  filename += nowTime.minute();
  if (nowTime.second() < 10) {
    filename += "0";
  }
  filename += nowTime.second();
  filename += ".png";

  // convert file string to character array and open the file for writing
  char filename2[30];
  filename.toCharArray(filename2, 30);
  logFile = SD.open(filename2, FILE_WRITE);
  //  return(logfile);
}
/********************************************************************/
// Function to provide a timestamp
DateTime timestamp() {
  // Written to work with the real-time clock that is integrated into an
  // Adafruit data logger shield.
  // Should also work with stand-alone clocks such as the Sparkfun RTC module
  // http://proto-pic.co.uk/real-time-clock-module/
  // as it's the same chip, provided the same arduino pins are used

  DateTime nowTime = RTC.now();
  return (nowTime);
}



// Flashes the signal light "x" times for "time" ms
void flashLight(int x, int time) {
  for (int i = 0; i < x; i++) {
    digitalWrite(GREENLIGHT, HIGH);
    delay(time);
    digitalWrite(GREENLIGHT, LOW);
    delay(time);
  }
}


// error signal
void sos(int x, int time) {
  for (int i = 0; i < x; i++) {
    digitalWrite(REDLIGHT, HIGH);
    delay(time);
    digitalWrite(REDLIGHT, LOW);
    delay(time);
  }

}


