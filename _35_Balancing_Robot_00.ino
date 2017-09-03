#include <Wire.h>                                           //Include the Wire.h library so we can communicate with the gyro
//Declaring global variables
byte start, received_byte, low_bat;

int left_motor, throttle_left_motor, throttle_counter_left_motor, throttle_left_motor_memory;
int right_motor, throttle_right_motor, throttle_counter_right_motor, throttle_right_motor_memory;
int battery_voltage;
int receive_counter;
int gyro_pitch_data_raw, gyro_yaw_data_raw, accelerometer_data_raw;

long gyro_yaw_calibration_value, gyro_pitch_calibration_value;

unsigned long loop_timer;

float angle_gyro, angle_acc, angle, self_balance_pid_setpoint;
float pid_error_temp, pid_i_mem, pid_setpoint, gyro_input, pid_output, pid_last_d_error;
float pid_output_left, pid_output_right;

boolean debug = true;


//Setup basic functions

void setup() {
  Serial.begin(9600);                                                       //Start the serial port at 9600 kbps
  Wire.begin();                                                             //Start the I2C bus as master
  TWBR = 12;                                                                //Set the I2C clock speed to 400kHz

  //To create a variable pulse for controlling the stepper motors a timer is created that will execute a piece of code (subroutine) every 20us
  //This subroutine is called TIMER2_COMPA_vect 
  TCCR2A = 0;                                                               //Make sure that the TCCR2A register is set to zero
  TCCR2B = 0;                                                               //Make sure that the TCCR2A register is set to zero
  TIMSK2 |= (1 << OCIE2A);                                                  //Set the interupt enable bit OCIE2A in the TIMSK2 register
  TCCR2B |= (1 << CS21);                                                    //Set the CS21 bit in the TCCRB register to set the prescaler to 8
  OCR2A = 39;                                                               //The compare register is set to 39 => 20us / (1s / (16.000.000MHz / 8)) - 1
  TCCR2A |= (1 << WGM21);                                                   //Set counter 2 to CTC (clear timer on compare) mode

  loop_timer = micros() + 4000;                                             //Set the loop_timer variable at the next end loop time

}


//Main program loop

void loop() {
  //Set the low_bat variable to 1
}


//Interrupt routine  TIMER2_COMPA_vect

ISR(TIMER2_COMPA_vect) {
  //Left motor pulse calculations
  throttle_counter_left_motor ++;
  //Increase the throttle_counter_left_motor variable by 1 every time this routine is executed
  if (debug) {
    Serial.println(throttle_counter_left_motor);
  }
  if (throttle_counter_left_motor > throttle_left_motor_memory) {           //If the number of loops is larger then the throttle_left_motor_memory variable
    throttle_counter_left_motor = 0;                                        //Reset the throttle_counter_left_motor variable
    throttle_left_motor_memory = throttle_left_motor;                       //Load the next throttle_left_motor variable
    if (throttle_left_motor_memory < 0) {                                   //If the throttle_left_motor_memory is negative
      PORTD &= 0b11110111;                                                  //Set output 3 low to reverse the direction of the stepper controller
      throttle_left_motor_memory *= -1;                                     //Invert the throttle_left_motor_memory variable
    }
    else PORTD |= 0b00001000;                                               //Set output 3 high for a forward direction of the stepper motor
  }
  else if (throttle_counter_left_motor == 1)PORTD |= 0b00000100;            //Set output 2 high to create a pulse for the stepper controller
  else if (throttle_counter_left_motor == 2)PORTD &= 0b11111011;            //Set output 2 low because the pulse only has to last for 20us

  //right motor pulse calculations
  throttle_counter_right_motor ++;                                          //Increase the throttle_counter_right_motor variable by 1 every time the routine is executed
  if (throttle_counter_right_motor > throttle_right_motor_memory) {         //If the number of loops is larger then the throttle_right_motor_memory variable
    throttle_counter_right_motor = 0;                                       //Reset the throttle_counter_right_motor variable
    throttle_right_motor_memory = throttle_right_motor;                     //Load the next throttle_right_motor variable
    if (throttle_right_motor_memory < 0) {                                  //If the throttle_right_motor_memory is negative
      PORTD |= 0b00100000;                                                  //Set output 5 low to reverse the direction of the stepper controller
      throttle_right_motor_memory *= -1;                                    //Invert the throttle_right_motor_memory variable
    }
    else PORTD &= 0b11011111;                                               //Set output 5 high for a forward direction of the stepper motor
  }
  else if (throttle_counter_right_motor == 1)PORTD |= 0b00010000;           //Set output 4 high to create a pulse for the stepper controller
  else if (throttle_counter_right_motor == 2)PORTD &= 0b11101111;           //Set output 4 low because the pulse only has to last for 20us
}


























