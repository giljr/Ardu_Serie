/*
   Project name: JayThree Time_Lapse CAM Project #arduSerie 36
     A very cool project that captures images for a predetermined time. 
     Serves as a time lapse camera. It is widely used in biology, 
     for the study of human or animal behavior. Have a good time!!!
     
     Flavour V - CAM & SDCard Dumping File 
     INO File: _36_Test_SerialCamera_CJ_OV528_05.ino
   
   Revision History:
     Oct, 2017
       - v 1.0 - Medium webpage:
   Description:
           This code initializes the SDCard, then initializes the camera, sets it up and takes pictures according to the 
           button pressing on the A5 port. Photos are saved in SDCard. Each photo has about 30-50 Bytes and if the card 
           has 1.85 GB, in these 30.000 photos of these size can be saved in this card.
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
/*
// _36_Test_SerialCamera_CJ_OV528_05
//  File SerialCamera_DemoCode_CJ-OV528.ino
//  8/8/2013 Jack Shao
//  Demo code for using seeeduino or Arduino board to cature jpg format
//  picture from seeed serial camera and save it into sd card. Push the
//  button to take the a picture .

//  For more details about the product 
//  please check https://github.com/Seeed-Studio/Grove_Serial_Camera_Kit
*/
#include <arduino.h>
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define PIC_PKT_LEN    128        //data length of each read, dont set this too big because ram is limited
#define PIC_FMT_VGA    7
#define PIC_FMT_CIF    5
#define PIC_FMT_OCIF   3
#define CAM_ADDR       0
#define CAM_SERIAL     softSerial

#define PIC_FMT        PIC_FMT_VGA


File myFile;
//SoftwareSerial softSerial(2, 3);  //rx,tx (11-13 is used by sd shield)
SoftwareSerial softSerial(20, 19);  //rx,tx for Arduino Pro Micro Generic
const byte cameraAddr = (CAM_ADDR << 5);  // addr
//const int buttonPin = A5;                 // the number of the pushbutton pin
const int buttonPin = 4;                 // for Arduino Pro Micro Generic
unsigned long picTotalLen = 0;            // picture length
int picNameNum = 0;
const int CS_PIN  =10;
/*********************************************************************/
void setup()
{
  Serial.begin(115200);
  CAM_SERIAL.begin(9600);       //cant be faster than 9600, maybe difference with diff board.
  pinMode(buttonPin, INPUT);    // initialize the pushbutton pin as an input
  Serial.println("Initializing SD card....");
  //pinMode(4,OUTPUT);          // CS pin of SD Card Shield
  pinMode(CS_PIN,OUTPUT);          // CS pin of SD Card Shield
  
  if (!SD.begin(CS_PIN)) {
    Serial.print("sd init failed");
    return;
  }
  Serial.println("sd init done.");
  initialize();
}
/*********************************************************************/
void loop()
{
  int n = 0;
  while(1){
    Serial.println("\r\nPress the button to take a picture");
    while (digitalRead(buttonPin) == LOW);      //wait for buttonPin status to HIGH
    Serial.println("taking");
    if(digitalRead(buttonPin) == HIGH){
      delay(20);                               //Debounce
      if (digitalRead(buttonPin) == HIGH)
      {
        delay(200);
        if (n == 0) preCapture();
        Capture();
        Serial.print("Saving picture...");
        GetData();
      }
      Serial.print("\r\nDone ,number : ");
      Serial.println(n);
      n++ ;
      }
    }
}
/*********************************************************************/
void clearRxBuf()
{
  while (CAM_SERIAL.available()) 
  {
    CAM_SERIAL.read(); 
  }
}
/*********************************************************************/
void sendCmd(char cmd[], int cmd_len)
{
  for (char i = 0; i < cmd_len; i++) CAM_SERIAL.write(cmd[i]); 
}
/*********************************************************************/
int readBytes(char *dest, int len, unsigned int timeout)
{
  int read_len = 0;
  unsigned long t = millis();
  while (read_len < len)
  {
    while (CAM_SERIAL.available()<1)
    {
      if ((millis() - t) > timeout)
      {
        return read_len;
      }
    }
    *(dest+read_len) = CAM_SERIAL.read();
    Serial.write(*(dest+read_len));
    read_len++;
  }
  return read_len;
}
/*********************************************************************/
void initialize()
{   
  char cmd[] = {0xaa,0x0d|cameraAddr,0x00,0x00,0x00,0x00} ;  
  unsigned char resp[6];

  Serial.print("initializing camera...");
  
  while (1) 
  {
    sendCmd(cmd,6);
    if (readBytes((char *)resp, 6,1000) != 6)
    {
      Serial.print(".");
      continue;
    }
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x0d && resp[4] == 0 && resp[5] == 0) 
    {
      if (readBytes((char *)resp, 6, 500) != 6) continue; 
      if (resp[0] == 0xaa && resp[1] == (0x0d | cameraAddr) && resp[2] == 0 && resp[3] == 0 && resp[4] == 0 && resp[5] == 0) break; 
    }
  }  
  cmd[1] = 0x0e | cameraAddr;
  cmd[2] = 0x0d;
  sendCmd(cmd, 6); 
  Serial.println("\nCamera initialization done.");
}
/*********************************************************************/
void preCapture()
{
  //config preview image, size and color type  0x05()
  char cmd[] = { 0xaa, 0x01 | cameraAddr, 0x00, 0x07, 0x00, PIC_FMT };  
  unsigned char resp[6]; 
  
  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) continue; 
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x01 && resp[4] == 0 && resp[5] == 0) break; 
  }
}
void Capture()
{
  char cmd[] = { 0xaa, 0x06 | cameraAddr, 0x08, PIC_PKT_LEN & 0xff, (PIC_PKT_LEN>>8) & 0xff ,0}; 
  unsigned char resp[6];

  while (1)
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) continue;
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
    if (readBytes((char *)resp, 6, 100) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x05 && resp[4] == 0 && resp[5] == 0) break;
  }
  cmd[1] = 0x04 | cameraAddr;
  cmd[2] = 0x1;
  while (1) 
  {
    clearRxBuf();
    sendCmd(cmd, 6);
    if (readBytes((char *)resp, 6, 100) != 6) continue;
    if (resp[0] == 0xaa && resp[1] == (0x0e | cameraAddr) && resp[2] == 0x04 && resp[4] == 0 && resp[5] == 0)
    {
      if (readBytes((char *)resp, 6, 1000) != 6)
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
  if ((picTotalLen % (PIC_PKT_LEN-6)) != 0) pktCnt += 1;
  
  char cmd[] = { 0xaa, 0x0e | cameraAddr, 0x00, 0x00, 0x00, 0x00 };  
  unsigned char pkt[PIC_PKT_LEN];
  
  char picName[] = "pic00.jpg";
  picName[3] = picNameNum/10 + '0';
  picName[4] = picNameNum%10 + '0';
  
  if (SD.exists(picName))
  {
    SD.remove(picName);
  }
  
  myFile = SD.open(picName, FILE_WRITE); 
  if(!myFile){
    Serial.println("myFile open fail...");
  }
  else{
    for (unsigned int i = 0; i < pktCnt; i++)
    {
      cmd[4] = i & 0xff;
      cmd[5] = (i >> 8) & 0xff;
      
      int retry_cnt = 0;
    retry:
      delay(10);
      clearRxBuf(); 
      sendCmd(cmd, 6); 
      uint16_t cnt = readBytes((char *)pkt, PIC_PKT_LEN, 200);
      
      unsigned char sum = 0; 
      for (int y = 0; y < cnt - 2; y++)
      {
        sum += pkt[y];
      }
      if (sum != pkt[cnt-2])
      {
        if (++retry_cnt < 100) goto retry;
        else break;
      }
      
      myFile.write((const uint8_t *)&pkt[4], cnt-6); 
      //if (cnt != PIC_PKT_LEN) break;
    }
    cmd[4] = 0xf0;
    cmd[5] = 0xf0; 
    sendCmd(cmd, 6); 
  }
  myFile.close();
  picNameNum ++;
}

