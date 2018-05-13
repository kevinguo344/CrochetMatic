
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
const int STEP_PER_MM = 128;
const int MAX_RANGE_MM = 60;
const int MAXIMUM_STEPS = MAX_RANGE_MM * STEP_PER_MM;
//const int MIN_NEEDLE_LATCH_DELTA_STEP = -10 * STEP_PER_MM;
//const int MAX_NEEDLE_LATCH_DELTA_STEP = 28*STEP_PER_MM;
const int MIN_NEEDLE_LATCH_DELTA_STEP = 0;
const int MAX_NEEDLE_LATCH_DELTA_STEP = 30*STEP_PER_MM;
int current_needle;

int prev_needle_value;
int prev_latch_value;

int32_t current_stepper_pos;
int32_t current_latch_pos;

class Needle {
  public:
  int needle_steps;
  int latch_steps;
  bool inverted;
  
  void safe_update_servos();
  void set_as_active();

  // To be replaced by I2C data to reach a servo;
  Servo needle_servo;
  Servo latch_servo;
};

void Needle::safe_update_servos()
{
  latch_steps = latch_stepper_position();
  needle_steps = needle_stepper_position();
  //provides constrain for needle steps so needle doesn't go beyond maximum range 
  needle_steps = constrain(needle_steps, 0, MAXIMUM_STEPS);
  Serial.print("Needle steps: "); Serial.println(needle_steps/STEP_PER_MM);
  int max_safe_latch_steps = needle_steps + MAX_NEEDLE_LATCH_DELTA_STEP;
  max_safe_latch_steps = constrain(max_safe_latch_steps, 0, MAXIMUM_STEPS);
  Serial.print("Max latch steps: "); Serial.println(max_safe_latch_steps/STEP_PER_MM);
  int min_safe_latch_steps = needle_steps + MIN_NEEDLE_LATCH_DELTA_STEP;
  min_safe_latch_steps = constrain(min_safe_latch_steps, 0, MAXIMUM_STEPS);
  Serial.print("Min latch steps: "); Serial.println(min_safe_latch_steps/STEP_PER_MM);
  
  latch_steps = constrain(latch_steps, min_safe_latch_steps, max_safe_latch_steps);
  Serial.print("Latch steps: "); Serial.println(latch_steps/STEP_PER_MM);
  
  if(this->inverted){
    needle_servo.write(180 - ((180.0*needle_steps)/MAXIMUM_STEPS));
    latch_servo.write((180.0*latch_steps)/MAXIMUM_STEPS);
  }
  else{
    needle_servo.write(((180.0*needle_steps)/MAXIMUM_STEPS));
    latch_servo.write(180 - ((180.0*latch_steps)/MAXIMUM_STEPS));
  }

}

void Needle::set_as_active()
{
  set_needle_stepper_position(needle_steps);
  set_latch_stepper_position(latch_steps);
}


const int number_of_needles = 5;
Needle needles[number_of_needles];

//needle 0 {main: 0, latch: 1}
//needle 1 {main: 2, latch: 5}
//needle 2 {main: 6, latch: 7}
//needle 3 {main: 8, latch: 9}
//needle 4 {main: 10, latch: 11}
const int servo_pins[2*number_of_needles] = {0, 1, 2, 5, 6, 7, 8, 9, 10, 11};

void switch_active_needle(int old_needle, int new_needle)
{
  if (old_needle != new_needle && new_needle < number_of_needles && new_needle >= 0)
  {
    needles[old_needle].safe_update_servos();
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
    Serial.println(servo_pins[i*2]);
    needles[i].needle_servo.attach(servo_pins[i*2]);
    
    Serial.println(servo_pins[i*2 + 1]);
    needles[i].latch_servo.attach(servo_pins[i*2 + 1]);
    Serial.print("Inverted: ");
    if(i%2 == 0){
      needles[i].needle_servo.write(0);
      needles[i].latch_servo.write(180);
      needles[i].inverted = false;
      Serial.println("false");
    }
    else{
      needles[i].needle_servo.write(180);
      needles[i].latch_servo.write(0);
      needles[i].inverted = true;
      Serial.println("true");
    }
   
  }
  current_needle = 0;
  current_stepper_pos = 0;
  current_latch_pos = 180;
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
      Serial.print("Active needle is: "); Serial.println(current_needle);
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
  if(current_stepper_pos != needle_stepper_position() || current_latch_pos != latch_stepper_position()){
    needles[current_needle].safe_update_servos();
    Serial.print("Active needle is: "); Serial.println(current_needle);
    //Serial.print("Inverted: "); Serial.println(needles[current_needle].inverted);
    Serial.print(needle_stepper_position());  Serial.print("/");  Serial.println(latch_stepper_position());
    //Serial.print("Needle servo current place: ");  Serial.println(needles[current_needle].needle_servo.read());
    //Serial.print("Needle Servo attached: "); Serial.println((bool)needles[current_needle].needle_servo.attached());
    //Serial.print("Latch servo current place: ");  Serial.println(needles[current_needle].latch_servo.read());
    //Serial.print("Latch  Servo attached: "); Serial.println((bool)needles[current_needle].latch_servo.attached());
    //Serial.println("");
    current_stepper_pos = needle_stepper_position();
    current_latch_pos = latch_stepper_position();
  }
}


