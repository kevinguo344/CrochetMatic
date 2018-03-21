#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 600

/*
example output
//GCODE SETUP
M260 A64

//REST
M280 P1 S150
M280 P2 S600

//DELAY 500
G4 P500

//PULL DOWN
M280 P1 S525
M280 P2 S300

//DELAY 500
G4 P500

//CLOSE UP
M280 P1 S150
M280 P2 S312.5
*/

//uint8_t servonum = 2;
const int topServo = 0;
const int botServo = 1;

void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(60);
  yield();
}

long convertAngle(int a){
  Serial.println(map(a, 0, 180, SERVOMIN, SERVOMAX));
  return map(a, 0, 180, SERVOMIN, SERVOMAX);
}

void loop() {
  driver.setPWM(topServo, 0, convertAngle(0));
  //if bottom angle is at (180-65) = 115, top can only go to 90
  //
  driver.setPWM(botServo, 0, convertAngle(180));
  Serial.println("In loop");

  //when bottom angle is at 180, top angle will be at 60 to close completely and 155 to open completely
  //

  //absolute ceiling of angles:
  //    top:    155 degrees
  //    bottom: 180 degrees

  //absolute floor of angles:
  //    top:    0 degrees
  //    bottom: 0 degrees

  //relative ceiling of angles:
  //    top: +115 degrees of bottom

  //relative floor of angles:
  //    top: +0 degrees of bottom

}

void sequence(){
  rest(); delay(500);
  pullDown(); delay(500);
  closeUp(); delay(500); 
}

void rest(){
  driver.setPWM(topServo, 0, convertAngle(0));
  Serial.println("M280 P1 S150");
  driver.setPWM(botServo, 0, convertAngle(180));
  Serial.println("M280 P2 S600");
}

void spin(){
  for(int i=0; i<180; i+=1){
    driver.setPWM(topServo, 0, convertAngle(i));
    driver.setPWM(botServo, 0, convertAngle(180-i));
    delay(5);
  }

  for(int i=179; i>=0; i-=1){
    driver.setPWM(topServo, 0, convertAngle(i));
    driver.setPWM(botServo, 0, convertAngle(180-i));
    delay(5);
  }
}

void pullDown(){
  driver.setPWM(topServo, 0, convertAngle(150));
  Serial.println("M280 P1 S525");
  driver.setPWM(botServo, 0, convertAngle(60));
  Serial.println("M280 P2 S300");
}

void closeUp(){
  driver.setPWM(topServo, 0, convertAngle(0));
  Serial.println("M280 P1 S150");
  driver.setPWM(botServo, 0, convertAngle(65));
  Serial.println("M280 P2 S312.5");
}

void extendAllUp(){
  driver.setPWM(topServo, 0, convertAngle(120));
  driver.setPWM(botServo, 0, convertAngle(0));
}

