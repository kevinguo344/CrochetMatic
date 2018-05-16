
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

int prev_needle_value = 0;
int prev_latch_value = 0;

int32_t current_stepper_pos;
int32_t current_latch_pos;

class Needle {
  public:
  int needle_steps;
  int latch_steps;
  float needle_pos;
  float latch_pos;
  //bool inverted;
  
  void safe_update_servos();
  void set_as_active();
  void safe_update_needle(float transmitZ, float transmitE);

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
  
  //Serial.print("Needle steps: "); Serial.println(needle_steps/STEP_PER_MM);
  int max_safe_latch_steps = needle_steps + MAX_NEEDLE_LATCH_DELTA_STEP;
  max_safe_latch_steps = constrain(max_safe_latch_steps, 0, MAXIMUM_STEPS);
  int min_safe_latch_steps = needle_steps + MIN_NEEDLE_LATCH_DELTA_STEP;
  min_safe_latch_steps = constrain(min_safe_latch_steps, 0, MAXIMUM_STEPS);
  
  latch_steps = constrain(latch_steps, min_safe_latch_steps, max_safe_latch_steps);
  //Serial.print("Latch steps: "); Serial.println(latch_steps/STEP_PER_MM);
  float needleAngle = map(needle_steps, 0, MAXIMUM_STEPS, 0, 180);
  float latchAngle = map(latch_steps, 0, MAXIMUM_STEPS, 0, 180);
  
  Serial.print("Written Angles: "); Serial.print(needleAngle); Serial.print("/"); Serial.println(latchAngle);
  needle_servo.write(needleAngle);
  latch_servo.write(180 - latchAngle);
  /*
  if(this->inverted){
    //needle_servo.write(180 - ((180.0*needle_steps)/MAXIMUM_STEPS));
    //latch_servo.write((180.0*latch_steps)/MAXIMUM_STEPS);
    needle_servo.write(180 - needleAngle);
    latch_servo.write(latchAngle);
    //needle_servo.write(needleAngle);
    //latch_servo.write(180 - latchAngle);
  }
  else{
    //needle_servo.write(((180.0*needle_steps)/MAXIMUM_STEPS));
    //latch_servo.write(180 - ((180.0*latch_steps)/MAXIMUM_STEPS));
    needle_servo.write(needleAngle);
    latch_servo.write(180 - latchAngle);
  }
  */
}

void Needle::safe_update_needle(float transmitZ, float transmitE){
  needle_pos = transmitZ;
  latch_pos = transmitE;
  needle_steps = transmitZ * STEP_PER_MM;
  needle_steps = constrain(needle_steps, 0, MAXIMUM_STEPS);
  int max_safe_latch_steps = needle_steps + MAX_NEEDLE_LATCH_DELTA_STEP;
  max_safe_latch_steps = constrain(max_safe_latch_steps, 0, MAXIMUM_STEPS);
  int min_safe_latch_steps = needle_steps + MIN_NEEDLE_LATCH_DELTA_STEP;
  min_safe_latch_steps = constrain(min_safe_latch_steps, 0, MAXIMUM_STEPS);
  latch_steps = transmitE * STEP_PER_MM;
  latch_steps = constrain(latch_steps, min_safe_latch_steps, max_safe_latch_steps);

  float needleAngle = map(needle_steps, 0, MAXIMUM_STEPS, 0, 180);
  float latchAngle = map(latch_steps, 0, MAXIMUM_STEPS, 0, 180);
  
  needle_servo.write(needleAngle);
  Serial.print("Safe update Z:"); Serial.println(needleAngle);
  latch_servo.write(latchAngle);
  Serial.print("Safe update E:"); Serial.println(180 - latchAngle));
  /*
  if(this->inverted){
    needle_servo.write(180 - ((180.0*needle_steps)/MAXIMUM_STEPS));
    Serial.print("Safe update Z:"); Serial.println(180 - ((180.0*needle_steps)/MAXIMUM_STEPS));
    latch_servo.write((180.0*latch_steps)/MAXIMUM_STEPS);
    Serial.print("Safe update E:"); Serial.println((180.0*latch_steps)/MAXIMUM_STEPS);
    //needle_servo.write(((180.0*needle_steps)/MAXIMUM_STEPS));
    //latch_servo.write(180 - ((180.0*latch_steps)/MAXIMUM_STEPS));
  }
  else{
    needle_servo.write(((180.0*needle_steps)/MAXIMUM_STEPS));
    Serial.print("Safe update Z:"); Serial.println(((180.0*needle_steps)/MAXIMUM_STEPS));
    latch_servo.write(180 - ((180.0*latch_steps)/MAXIMUM_STEPS));
    Serial.print("Safe update E:"); Serial.println(180 - ((180.0*latch_steps)/MAXIMUM_STEPS));
  }
  */
}

void Needle::set_as_active()
{
  //set_needle_stepper_position(needle_steps);
  //set_latch_stepper_position(latch_steps);
  set_needle_stepper_position(prev_needle_value);
  set_latch_stepper_position(current_latch_pos);
}


const int number_of_needles = 3;
Needle needles[number_of_needles];

//needle 0 {main: 0, latch: 1}
//needle 1 {main: 2, latch: 5}
//needle 2 {main: 6, latch: 7}
const int servo_pins[2*number_of_needles] = {0, 1, 2, 5, 6, 7};

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
    needles[i].needle_servo.write(0);
    needles[i].latch_servo.write(180);
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
    //N1,60.00,60.00
    //  0   1   2   3   5   6   7   8   9   10  11 12  13  14
    //['N','1',' ','6','0','.','0','0',' ','6','0','.','0','0']
    //data array will be 14 long
    char command[14];
    for (int k = 0; k < 14; k++)
    {
      command[k] = Wire.read();
    }
    Serial.print("Recieved: ");
    Serial.print(command);
    Serial.println(" ");
    if (command[0] == 'N')
    {
      String to_parse = (command + 1);
      to_parse = to_parse.substring(1);
      String needleNum, zCoord, eCoord;
      for(int i = 0; i < to_parse.length(); i++){
        if(to_parse.charAt(i) == ','){
          needleNum = to_parse.substring(0,i);
          zCoord = to_parse.substring(i+1);
          break;
        }
      }
      for(int i = 0; i < zCoord.length(); i++){
        if(zCoord.charAt(i) == ','){
          eCoord = zCoord.substring(i+1);
          zCoord = zCoord.substring(0,i);
          break;
        }
      }
      long new_needle = needleNum.toInt();
      float z = zCoord.toFloat();
      float e = eCoord.toFloat();
      Serial.print("N: "); Serial.print(new_needle); Serial.print(" Z: "); Serial.print(z); Serial.print(" E: "); Serial.println(e);
      switch_active_needle(current_needle, new_needle);
      needles[current_needle].safe_update_needle(z, e);
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
    Serial.print("Inverted: "); Serial.println(needles[current_needle].inverted);
    Serial.print(needle_stepper_position());  Serial.print("/");  Serial.println(latch_stepper_position());
    Serial.print(needles[current_needle].needle_servo.read()); Serial.print(" deg / "); Serial.print(needles[current_needle].latch_servo.read()); Serial.println(" deg");
    current_stepper_pos = needle_stepper_position();
    current_latch_pos = latch_stepper_position();
  }
}

