
// Teensy pin 3 is step for main needle 
// Teensy pin 4 direction for main needle
// Teensy pin 29 is step for latch needle 
// Teensy pin 30 direction for latch needle
// Receive instructions on I2C0 (SCL0 -> 19 and SCA0 -> 18) or serial
// Send command to servos on I2C1 (SCL1 -> 37 and SCA1 -> 38)
#include "Encoder.h"
#include <Wire.h>
#include <Servo.h>
const int I2C_SLAVE = 42;
const int MAXIMUM_STEPS = 180;
int current_needle;

int32_t current_stepper_pos;
int32_t current_latch_pos;

class Needle {
  public:
  int needle_steps;
  int latch_steps;

  void update_needle_servo_value();
  void update_latch_servo_value();
  void update_servos();

  void set_as_active();

  // To be replaced by I2C data to reach a servo;
  Servo needle_servo;
  Servo latch_servo;
};


void Needle::update_needle_servo_value()
{
  needle_steps = needle_stepper_position();
  //Serial.print("Needle steps: ");
  //Serial.println(needle_steps);
  // To be replaced by I2C commands
  needle_servo.write((180.0*needle_steps)/MAXIMUM_STEPS);
}

void Needle::update_latch_servo_value()
{
  latch_steps = latch_stepper_position();
  //Serial.print("Latch steps: ");
  //Serial.println(latch_steps);
  // To be replaced by I2C commands
  // Wire.beginTransmission(... // address of the target board
  // Wire.write(...
  // Wire.write(...
  // Wire.endTransmission();    // Transmit the information...

  latch_servo.write((180.0*latch_steps)/MAXIMUM_STEPS);
}

void Needle::update_servos()
{
  update_needle_servo_value();
  update_latch_servo_value();
}

void Needle::set_as_active()
{
  set_needle_stepper_position(needle_steps);
  set_latch_stepper_position(latch_steps);
}


const int number_of_needles = 2;
Needle needles[number_of_needles];
const int servo_pins[2*number_of_needles] = {0, 1, 5, 6};

void switch_active_needle(int old_needle, int new_needle)
{
  if (old_needle != new_needle)
  {
    needles[old_needle].update_servos();
    needles[new_needle].set_as_active();
    current_needle = new_needle;
  }
}

void setup()
{
  encoder_setup();
  for (int i = 0; i < number_of_needles; i++)
  {
    // Here we initialize the different paramters for each servo of that needle
    needles[i].needle_servo.attach(servo_pins[i*2]);
    needles[i].needle_servo.write(0);

    needles[i].latch_servo.attach(servo_pins[i*2 + 1]);
    needles[i].latch_servo.write(0);
  }
  current_needle = 0;
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE);
  Wire.onReceive(on_I2C_event);
  Wire1.begin();
}

void on_I2C_event(int how_many)
{
  if (how_many >= 4)
  {
    char command[5];
    for (int k = 0; k < 4; k++)
    {
      command[k] = Wire.read();
    }
    command[4] = 0;
    Serial.print("Recieved: ");
    Serial.print(command);
    Serial.print(" ");
    if (command[0] == 'N')
    {
      String to_parse = (command + 1);
      long new_needle = to_parse.toInt();
      switch_active_needle(current_needle, new_needle);
      Serial.print("Active needle is: ");
      Serial.println(current_needle);
    }
    else
    {
      Serial.println("Cannot parse the I2C command!");
    }
  }
  else
  {
    Serial.println(how_many);
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    switch(Serial.read())
    {
      case '0':
        switch_active_needle(current_needle, 0);
        current_needle = 0;
      break;
      case '1':
        switch_active_needle(current_needle, 1);
        current_needle = 1;
      break;
      default:
      break;
    }
    Serial.print("Active needle is: ");
    Serial.print(current_needle);
  }
  needles[current_needle].update_servos();
  if(current_stepper_pos != needle_stepper_position() || current_latch_pos != latch_stepper_position()){
    Serial.print(needle_stepper_position());
    Serial.print("/");
    Serial.println(latch_stepper_position());
    current_stepper_pos = needle_stepper_position();
    current_latch_pos = latch_stepper_position();
  }
}


