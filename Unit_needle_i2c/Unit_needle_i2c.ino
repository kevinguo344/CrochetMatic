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

uint8_t servonum = 2;
const int topServo = 0;
const int botServo = 1;

void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(60);
  yield();
  rest();
  Serial.println("M260 A64");
}

long convertAngle(int a){
  Serial.println(map(a, 0, 180, SERVOMIN, SERVOMAX));
  return map(a, 0, 180, SERVOMIN, SERVOMAX);
}

void loop() {
  //rest();
  pullDown();
  //closeUp();
  //sequence();
  //spin();
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
