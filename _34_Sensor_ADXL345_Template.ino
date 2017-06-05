// ARDUINO PSEUDOCODE FOR 3-AXIS SENSORS
#define SENSOR 0x69; // Device add in which is also included the 8th bit for selectting the mode, read in this case
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
#include <wire.h>
void setup() {
  // 1 - Which device (address) are you interest in?
  // Initialize the wire library
  Wire.begin(9600);
  // begin transmission
  Wire.beginTransmission(SENSOR);
  // 2 - Which Register do you want to talk to?
  Wire.write(0x2D); // Power-saving features control
  // Register 0x2D POWER_CTL is 8 bits: [unused:unused:Link:AUTO_SLEEP:Measure:Sleep:Wakeup1:Wakeup0]
  // 3 - What do you want to transmit? Enable measurement
  Wire.write(8);   // bit D3 high for measuring enable: 8d - 00001000b
  // end Transmission
  Wire.endTransmission();
}

void loop() {
  // begin transmission
  Wire.beginTransmission(SENSOR);
  // 4 -What do you want to ask the specific register ?
  Wire.write(Required_Register_DATA_0);
  Wire.write(Required_Register_DATA_1);
  // end Transmission
  Wire.endTransmission();
  // 5 - Now wait for the data looping in a while !
  Wire.requestFrom(Sensor_Address, 2);
  if (Wire.available() <= 2) {
    DataReturned_0 = Wire.read();
    DataReturned_1 = Wire.read();
    // 6 - Now how do you want the configuration of the three axes ?
    //  now read the data and do as the datasheet says.
    DataReturned_1 = DataReturned_1 << 8;
    DataReturned_1_out = DataReturned_0 + DataReturned_1;
    Data_actual = DataReturned_1_out / 256.0;
  }
