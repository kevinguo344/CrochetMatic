#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 600

uint8_t servonum = 2;
const int topServo = 0;
const int botServo = 1;

void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(60);
  yield();
  rest();
}

long convertAngle(int a){
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
  driver.setPWM(botServo, 0, convertAngle(180));
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
  driver.setPWM(botServo, 0, convertAngle(60));
}

void closeUp(){
  driver.setPWM(topServo, 0, convertAngle(0));
  driver.setPWM(botServo, 0, convertAngle(65));
}
