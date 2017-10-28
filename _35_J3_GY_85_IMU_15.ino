/*
   Project name: JayThree Balancing Car Project #arduSerie 35
     Awesome project based on Your Arduino Balancing Robot (YABR) by Joop Brokking
     Flavour XV - Working with GY-85 Board as automatic level instrument
     INO File: _35_J3_GY85_IMU_15.ino
   Revision History:
     Sept, 2017
       - v 1.0 - Medium webpage: https://goo.gl/4zmgGK
   Description:
         This code is used to establish or verify angles in some plane. It's for J3BC proj!
         This code is very long and sophisticated and was largely based and recompiled 
         from this page http://www.brokking.net/imu.html code for migration of the IMU GY-521 board
         to the GY-85 board. It's an initial version. We accept collaborations:D
         Try it youself! Visit our webpage!
   MCU:                  Pro Mini Atmega328 16MHz 5V @16MHz -  https://www.sparkfun.com/products/11113
   IDE:                  Arduino 1.8.3 - http://www.arduino.cc/
   IMU:                  9-DOF IMU module - GY-85
                         http://ianglen.me/misc/arduino-module-buying-guide/#gy-85
   Connections:          See Official youtube channel vids: https://goo.gl/R25hLc
   Based on:             Your Arduino Balancing Robot (YABR) by Joop Brokking. http://www.brokking.net/yabr_main.html
   Credit:               Joop Brokking                   
   Data Sheet ITG3205:   http://www.datasheetspdf.com/PDF/ITG3205/731955/1
   Terms of use:  
     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
     THE SOFTWARE.  
  
  //original notes:
  Website: http://www.brokking.net/imu.html
  Youtube: https://youtu.be/4BoIE8YQwM8
  Version: 1.0 (May 5, 2016)
  ///////////////////////////////////////////////////////////////////////////////////////
  //Connections
  ///////////////////////////////////////////////////////////////////////////////////////
  Power (5V) is provided to the Arduino pro mini by the FTDI programmer

  Gyro - Arduino pro mini
  VCC  -  5V
  GND  -  GND
  SDA  -  A4
  SCL  -  A5

  LCD  - Arduino pro mini
  VCC  -  5V
  GND  -  GND
  SDA  -  A4
  SCL  -  A5
*//////////////////////////////////////////////////////////////////////////////////////

//Include LCD and I2C library
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//-------- REGISTER MAP for ADXL345 chip   -----//
#define ACC         (0x53)                      // ADXL345 ACC address
#define POWER_CTL   (0x2D)                      // Power-saving features control 
#define DATA_FORMAT (0x31)                      // Data format control 
#define BW_RATE     (0x2C)                      // Data rate and power mode control 
#define DATAX0      (0x32)                      // First address for axis x
#define A_TO_READ   (6)                         // num of bytes we are going to read each time 
                                                // two bytes for each axis

//-------- REGISTER MAP for ITG3200 chip   -----// Gyroscope MPU3050 / ITG3200 / ITG3205
#define GYRO        (0x68)                      // when AD0 is connected to GND ,gyro address is 0x68; GY-85
//#define GYRO 0x69                             // when AD0 is connected to VCC ,gyro address is 0x69
#define PWR_MGM     (0x3E)                      // Register 3E – Power Management Control
                                                //   used to manage the power control, select the clock source,
                                                //   and to issue a master reset to the device - pg 27
#define SMPLRT_DIV  (0x15)                      // Register 21 – Sample Rate Divider
                                                //   determines the sample rate of the ITG-3205 gyros
#define DLPF_FS     (0x16)                      // Register 22 – DLPF, Full Scale
                                                //   configures several parameters related to the sensor acquisition.
                                                //   The FS_SEL parameter allows setting the full-scale range of the gyro sensors
                                                //   The power-on-reset value of FS_SEL is 00h. Set to 03h for proper operation.
#define INT_CFG     (0x17)                      // Register 23 – Interrupt Configuration
                                                //   configures the interrupt operation of the device.
#define G_TO_READ   (8)                         // 2 bytes for each axis x, y, z
#define TEMP_OUT_H  (0x1B)                      // 16-bit temperature data (2’s complement format)

//------------   GLOBALS VARIABLES  ------------//
int       gyro_x, gyro_y, gyro_z;               // Gyro raw data
long      acc_x, acc_y, acc_z, acc_total_vector;// Accel raw data
int       temperature;                          // Temp data; not used here:b
long      gyro_x_cal, gyro_y_cal, gyro_z_cal;   // Calibration routines data
long      loop_timer;                           // Loop time control 25Hz
int       lcd_loop_counter;
float     angle_pitch, angle_roll;              // used in calc routine
int       angle_pitch_buffer, angle_roll_buffer;// Angles buffers
boolean   set_gyro_angles;
float     angle_roll_acc, angle_pitch_acc;      // Angles returned
float     angle_pitch_output, angle_roll_output;// Row angles
int       acc[3];                               // For retrieval of information from chips
int       gyro[4];

//-------  INITIALIZE THE LCD LIBRARY  --------//
//https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);    // 0x27 is the I2C bus address for an unmodified backpack

//-------------------  SETUP  -----------------//
void setup() {
  Wire.begin();                                // Start I2C as master
  Serial.begin(57600);                         // Use only for debugging
  pinMode(13, OUTPUT);                         // Set output 13 (LED) as output

  config_ADXL345_ITG3205_GY85();               // Config the chips' registers of the GY85-BOARD (2000dfs / +/-8g) 

  digitalWrite(13, HIGH);                      // Set digital output 13 high to indicate startup

  lcd.begin(16, 2);                            // Initialize the LCD
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);                      // Activate backlight
  lcd.clear();                                 // Clear the LCD

  lcd.setCursor(0, 0);                         // Set the LCD cursor to position to position 0,0
  lcd.print("J3_GY85_IMU v1.0");               // Print text to screen
  lcd.setCursor(0, 1);                         // Set the LCD cursor to position to position 0,1
  lcd.print("(c) brokking.net");                       //Print text to screen

  delay(2500);                                 //Delay 1.5 second to display the text
  lcd.clear();                                 //Clear the LCD

  lcd.setCursor(0,0);                          //Set the LCD cursor to position to position 0,0
  lcd.print("Calibrating gyro");               //Print text to screen
  lcd.setCursor(0,1);                          //Set the LCD cursor to position to position 0,1
  for (int cal_int = 0; cal_int < 2000 ; cal_int ++){                  //Run this code 2000 times
    if(cal_int % 125 == 0)lcd.print(".");      //Print a dot on the LCD every 125 readings
    read_mpu_GY85_data();                      //Read the raw acc and gyro data from the GY85-BOARD
    gyro_x_cal += gyro_x;                      //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                      //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                      //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);                                  //Delay 3us to simulate the 250Hz program loop
  }
  gyro_x_cal /= 2000;                          //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 2000;                          //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 2000;                          //Divide the gyro_z_cal variable by 2000 to get the avarage offset
    
  lcd.clear();                                 //Clear the LCD

  lcd.setCursor(0, 0);                         //Set the LCD cursor to position to position 0,0
  lcd.print("Pitch:");                         //Print text to screen
  lcd.setCursor(0, 1);                         //Set the LCD cursor to position to position 0,1
  lcd.print("Roll :");                         //Print text to screen

  digitalWrite(13, LOW);                       //All done, turn the LED off

  loop_timer = micros();                       //Reset the loop timer
}

//----------------------  LOOP  ---------------//
void loop() {

  read_mpu_GY85_data(); 
 
  angle_calculations_routine();                // Does all the calculations to spit angles. Watch the videos to better understand!
  
                                               //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  
   write_LCD();                                //Write the roll and pitch values to the LCD display
  
  while (micros() - loop_timer < 4000);        //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
  loop_timer = micros();                       //Reset the loop timer
}

//-----------  ANGLE CALCULATIONS -------------//
void angle_calculations_routine() {
  gyro_x -= gyro_x_cal;                        //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                        //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                        //Subtract the offset calibration value from the raw gyro_z value

                                               //Gyro angle calculations
  //0.0000611 = 1 / (250Hz / 65.5)
  angle_pitch += gyro_x * 0.0000611;           //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0000611;            //Calculate the traveled roll angle and add this to the angle_roll variable

  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
  
//não funociona:
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //Calculate the roll angle
  
  //Place the GY85-BOARD spirit level and note the values in the following two lines for calibration
  angle_pitch_acc -= 0.0;                                              //Accelerometer calibration value for pitch
  angle_roll_acc -= 0.0;                                               //Accelerometer calibration value for roll

  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else{                                        //At first start
    angle_pitch = angle_pitch_acc;             //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;               //Set the gyro roll angle equal to the accelerometer roll angle 
    set_gyro_angles = true;                    //Set the IMU started flag
  }
}

//---------   CONFIG ACCEL & GYROS CHIPS ------//
void config_ADXL345_ITG3205_GY85() {
  
  // Data Sheet ADXL345: http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf
  // ACCELEROMETER SETUP
  
  /*Register 0x2D—POWER_CTL (Read/Write)
    [D7] [D6] [D5]   [D4]         [D3]      [D2]    [D1  D0]
    0    0    Link   AUTO_SLEEP   Measure*  Sleep   Wakeup    */
  writeTo(ACC, POWER_CTL,   0B00001000);       // 1 places the part into measurement mode
  /*Register 0x31—DATA_FORMAT (Read/Write)
    [D7]       [D6]  [D5]       [D4]  [D3]       [D2]     [D1 D0]
    SELF_TEST  SPI   INT_INVERT  0    FULL_RES*  Justify  Range**   */
  writeTo(ACC, DATA_FORMAT, 0B00001010);       // +-8g range, right-justified, full resolution
  /*Register 0x2C—BW_RATE (Read/Write)
    [D7] [D6] [D5] [D4]        [D3 D2 D1 D0]
    0     0    0   LOW_POWER   Rate***         */
  writeTo(ACC, BW_RATE,     0B00101100);       // 1100 -> ODR = 400Hz, 200 bandwidth = ODR/2
                                               // (ODR = Output Data Rate); Table 7 & 8 DS
                                               // same outputting data at each 10 ms (T = 1/ F)

  //Data Sheet ITG3205: http://www.datasheetspdf.com/PDF/ITG3205/731955/1
  // GYROSCOPE SETUP

  /*PWR_MGM Parameters:
    H_RESET Reset device and internal registers to the power-up-default settings
    SLEEP   Enable low power sleep mode
    STBY_XG Put gyro X in standby mode (1=standby, 0=normal)
    STBY_YG Put gyro Y in standby mode (1=standby, 0=normal)
    STBY_ZG Put gyro Z in standby mode (1=standby, 0=normal)
    CLK_SEL Select device clock source

    Register 62 – Power Management - pg 27
    [B7]    [B6]   [B5]   [B4]   [B3]      [B2  B1   B0]
    H_RESET SLEEP  STBY   _XG    STBY_YG   STBY*/
  writeTo(GYRO, PWR_MGM,    0B00000000);       //  int osc, no rst, no sleep, normal mode
  /*SMPLRT_DIV Formula:
    The gyros outputs are sampled internally at either 1kHz or 8kHz, determined by the DLPF_CFG
    Fsample = Finternal / (divider+1), where Finternal is either 1kHz or 8kHz
    Sample rate divider: 0 to 255

    Register 21 – Sample Rate Divider - pg 23
    [B7    B6   B5   B4   B3   B2  B1   B0]
    SMPLRT_DIV ***                             */
  writeTo(GYRO, SMPLRT_DIV, 0B00000111);       // divider is 7d
  /*DLPF_FS Parameters:
    FS_SEL   Full scale selection for gyro sensor data
    DLPF_CFG Digital low pass filter configuration and internal sampling rate configuration

    Register 22 – DLPF, Full Scale - pg 24
    [B7    B6   B5   [B4   B3]   [B2  B1   B0]
     -      -   -    FS_SEL**    DLPF_CFG**    */
  writeTo(GYRO, DLPF_FS,    0B00011110);       // +/-2000 DPS, DLPF=5Hz, Int.sampleRate=1KHz,
  /*INT_CFG Parameters:
    ACTL             Logic level for INT output pin – 1=active low, 0=active high
    OPEN             Drive type for INT output pin – 1=open drain, 0=push-pull
    LATCH_INT_EN     Latch mode – 1=latch until interrupt is cleared, 0=50us pulse
    INT_ANYRD_2CLEAR Latch clear method – 1=any register read, 0=status register read only
    0                Load zeros into Bit 3 of the Interrupt Configuration register
    ITG_RDY_EN       Enable interrupt when device is ready (PLL ready after changing clock source)
    0                Load zeros into Bit 1 of the Interrupt Configuration register
    RAW_RDY_EN       Enable interrupt when data is available


    Register 22 – DLPF, Full Scale - pg 24
    [B7]    [B6]   [B5]          [B4]               [B3]   [B2]        [B1]   [B0]
    ACTL   OPEN   LATCH_INT_EN  INT_ ANYRD_2CLEAR   0     ITG_RDY_EN   0     RAW_  RDY_ EN */
  writeTo(GYRO, INT_CFG,    0B00000000);       // logic high, push-pull, no latch, no int
}


//---   READING RAW DATA FROM ACCEL & GYRO ----//
void read_mpu_GY85_data() {                    //Subroutine for reading the raw gyro and accelerometer data

  byte addr;
  getGyroscopeData(gyro);
  gyro_x = gyro[0] / 14.375;
  gyro_y = gyro[1] / 14.375;
  gyro_z = gyro[2] / 14.375;
  temperature = 35 + ((double) (gyro[3] + 13200)) / 280; // temperature
  
  getAccelerometerData(acc);
  acc_x = acc[0];
  acc_y = acc[1];
  acc_z = acc[2];
  
}

//--------  GYRO RETURN DATA FUNCTION  --------//
void getGyroscopeData(int * result)
{
  int regAddress = TEMP_OUT_H;
  int temp, x, y, z;
  byte buff[G_TO_READ];
  readFrom(GYRO, regAddress, G_TO_READ, buff); //read the gyro data from the ITG3200
  result[0] = ((buff[2] << 8) | buff[3]);
  result[1] = ((buff[4] << 8) | buff[5]);
  result[2] = ((buff[6] << 8) | buff[7]);
  result[3] = (buff[0] << 8) | buff[1];        // temperature
}


//-------- ACCEL RETURN DATA FUNCTION  --------//
void getAccelerometerData(int * result) {

  int regAddress = DATAX0;                     // first axis-acceleration-data register on the ADXL345
  byte buff_acc[A_TO_READ];                    // simultaneous reading as recommended by the data sheet
  readFrom(ACC, regAddress, A_TO_READ, buff_acc);   // read the acceleration data from the ADXL345
                                               // each axis reading comes in 10 bit resolution
                                               // ie 2 bytes. Least Significat Byte first!!
                                               // thus we are converting both bytes in to one int
  result[0] = (((int)buff_acc[1]) << 8) | buff_acc[0];   
  result[1] = (((int)buff_acc[3])<< 8) | buff_acc[2];
  result[2] = (((int)buff_acc[5]) << 8) | buff_acc[4];
  
}

//------------  I2C WRITE FUNCTION  -----------//
void writeTo(int DEVICE, byte address, byte val) {

  Wire.beginTransmission(DEVICE);              // start transmission to ACC
  Wire.write(address);                         // send register address
  Wire.write(val);                             // send value to write
  Wire.endTransmission();                      // end transmission

}
//------------   I2C READ FUNCTION  -----------//
// reads num bytes starting from address
// register on ACC in to buff array
void readFrom(int DEVICE, byte address, int num, byte buff[]) {

  Wire.beginTransmission(DEVICE);              // start transmission to ACC
  Wire.write(address);                         // sends address to read from
  Wire.endTransmission();                      // end transmission

  //Wire.beginTransmission(DEVICE);            // start transmission to ACC
  Wire.requestFrom(DEVICE, num);               // request 6 bytes from ACC
  int i = 0;
  while (Wire.available())                     // ACC may send less than requested (abnormal)
  {
    buff[i] = Wire.read();                     // receive a byte
    i++;
  }
  //Wire.endTransmission();                    // end transmission

}

//------------  LCD WRITE FUNCTION ------------//
void write_LCD() {                             // Subroutine for writing the LCD
  //To get a 250Hz program loop (4us) it's only possible to write one character per loop
  //Writing multiple characters is taking to much time
  if (lcd_loop_counter == 14)lcd_loop_counter = 0;                     // Reset the counter after 14 characters
  lcd_loop_counter ++;                         //Increase the counter
  if (lcd_loop_counter == 1) {
    angle_pitch_buffer = angle_pitch_output * 10;                      // Buffer the pitch angle because it will change
    lcd.setCursor(6, 0);                                               // Set the LCD cursor to position to position 0,0
  }
  if (lcd_loop_counter == 2) {
    if (angle_pitch_buffer < 0)lcd.print("-");                         // Print - if value is negative
    else lcd.print("+");                       // Print + if value is negative
  }
  if (lcd_loop_counter == 3)lcd.print(abs(angle_pitch_buffer) / 1000); // Print first number
  if (lcd_loop_counter == 4)lcd.print((abs(angle_pitch_buffer) / 100) % 10); // Print second number
  if (lcd_loop_counter == 5)lcd.print((abs(angle_pitch_buffer) / 10) % 10); // Print third number
  if (lcd_loop_counter == 6)lcd.print(".");                            // Print decimal point
  if (lcd_loop_counter == 7)lcd.print(abs(angle_pitch_buffer) % 10);   // Print decimal number

  if (lcd_loop_counter == 8) {
    angle_roll_buffer = angle_roll_output * 10;
    lcd.setCursor(6, 1);
  }
  if (lcd_loop_counter == 9) {
    if (angle_roll_buffer < 0)lcd.print("-");  // Print - if value is negative
    else lcd.print("+");                       // Print + if value is negative
  }
  if (lcd_loop_counter == 10)lcd.print(abs(angle_roll_buffer) / 1000); // Print first number
  if (lcd_loop_counter == 11)lcd.print((abs(angle_roll_buffer) / 100) % 10); // Print second number
  if (lcd_loop_counter == 12)lcd.print((abs(angle_roll_buffer) / 10) % 10); // Print third number
  if (lcd_loop_counter == 13)lcd.print(".");   //Print decimal point
  if (lcd_loop_counter == 14)lcd.print(abs(angle_roll_buffer) % 10);   // Print decimal number
}









