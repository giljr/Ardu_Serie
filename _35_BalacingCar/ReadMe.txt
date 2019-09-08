Description of the Files:

file1: _35_Balancing_Robot_00.ino   

file2: _35_Balancing_Robot_5_Butts_01.ino   

file3: _35_ProMini_StepDownConverter_DRV8825_StepperMotor_02.ino   

File4: _35_ProMini_StepDownConverter_DRV8825_StepperMotor_BatMon_03.ino

File5: _35_GY85_ADXL345_Accelerometer_06.ino  
This code simplifies sensor’s data acquisition and does this simultaneously, as recommended by the data sheet! forms a code        pattern for the other sensors. It deserves increased attention!

File6: _35_GY85__ITG3205_Gyroscope_07.ino
Notice the definitions. They follow the names of data sheet’s real registers.

File7: _35_J3BC_Hardware_Scanner_10.ino.in
Based on the Arduino Scanner code, it rotates at the I2C addresses to discover the components of the GY-85 board and Wii Nunchuk. It should be adapted if you use another IMU (Inertial Measurement Unit) board other than GY-85 .
File8:  _35_ArduinoNunchukDemo_09.ino.in
Follow the improved Wii Nunchuk library for Arduino of GabrielBianconi/arduino-nunchuk. It works like a breeze!

File9:  _35_Sending_Serial_Counter_12.ino
This code simply sends a count from Arduino UNO for initial tests of the radio transmitting.

File10: _35_Receiving_Serial_Data_11.ino
This code receives the data that arrives at the serial of the Arduino Pro Mini.

File11: _35_GY85_HMC5883L_Magnetometer_08.ino
and for this documentation to be complete, I also offer the code for the magnetometer, although not used in this project (who knows in another version?;)
