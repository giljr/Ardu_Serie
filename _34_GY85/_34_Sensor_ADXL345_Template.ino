//Edited: 
//Sept, 2017
//       v 1.1 - Removed: if (Wire.available() <= 1) will always true - it can be removed 
//               Thanks to https://github.com/Koepel
// ARDUINO six-steps PSEUDOCODE FOR 3-AXIS SENSORS ADXL345 
#include <Wire.h>
/*
  ADXL345 Datasheet: https://goo.gl/uOZc2b
  Table 16. Register Map Address
  Hex   Dec Name  Type  Reset_Value   Description
  0x2D  45 POWER_CTL  R/W   00000000  Power-saving features control.
  (...)
  0x32  50 DATAX0   R   00000000  X-Axis Data 0.
  0x33  51 DATAX1   R   00000000  X-Axis Data 1.
  0x34  52 DATAY0   R   00000000  Y-Axis Data 0.
  0x35  53 DATAY1   R   00000000  Y-Axis Data 1.
  0x36  54 DATAZ0   R   00000000  Z-Axis Data 0.
  0x37  55 DATAZ1   R   00000000  Z-Axis Data 1.
*/
#define SENSOR 0x53 // Device add in which is also included the 8th bit for selectting the mode, read in this case
#define Power_Register 0x2D
#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Y_Axis_Register_DATAY0 0x34
#define Y_Axis_Register_DATAY1 0x35
#define Z_Axis_Register_DATAZ0 0x36
#define Z_Axis_Register_DATAZ1 0x37

int  DataReturned_x0, DataReturned_x1, DataModified_x_out;
float x;

void setup(){
                                        // Initialize the wire library baudrate: 9600
  Wire.begin(9600);
  Serial.begin(9600);
  delay(100);
                                        // 1 - Which device (address) are you interest in ?                                   
  Wire.beginTransmission(SENSOR);       // Begin transmission with the chosen chip
                                        // 2 - Which Register do you want to talk to ?
  Wire.write(Power_Register);           // Power-saving features control
                                        // Register 0x2D POWER_CTL is 8 bits: [unused:unused:Link:AUTO_SLEEP:Measure:Sleep:Wakeup1:Wakeup0]
                                        // 3 - What do you want to transmit? Enable measurement
  Wire.write(8);                        // bit D3 high for measuring enable: 8d - 00001000b
  
  Wire.endTransmission();               // end Transmission
}

void loop() {
  
 //-------------------------------------REAPEAT THIS PIECE OF CODE FOR EACH AXIS---------------------------------------//
  
  Wire.beginTransmission(SENSOR);       // begin transmission
                                        // 4 -What do you want to ask about the specific register ?
  Wire.write(X_Axis_Register_DATAX0);   // Requiring Register DATA_0 
  Wire.write(X_Axis_Register_DATAX1);   // Requiring Register DATA_1
                                        
  Wire.endTransmission();               // end Transmission
                                        // 5 - Now wait for the data looping in a while ...
  Wire.requestFrom(SENSOR, 2);
  //if (Wire.available() <= 2) {
    DataReturned_x0 = Wire.read();
    DataReturned_x1 = Wire.read();
                                        // 6 - Now how do you want the configuration of the three axes ?
                                        //  now read the data and do as the datasheet says.
    DataReturned_x1    = DataReturned_x1 << 8;
    DataModified_x_out = DataReturned_x0 + DataReturned_x1;
    x = DataModified_x_out / 256.0;
    
    //----------------------------------------------------------------------------------------------------------------//
  
  //}
                                        // Prints the data on the Serial Monitor
  Serial.print("x = ");
  Serial.println(x);
}
  
