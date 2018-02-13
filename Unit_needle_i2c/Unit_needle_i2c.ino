#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();

#define SERVOMIN 500
#define SERVOMAX 2400

uint8_t servonum = 2;
const int topServo = 0;
const int botServo = 1;

void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(60);
  yield();
}

void loop() {
  sequence();
}

long convertToPulse(int a){
  return map(a, 0, 180, SERVOMIN, SERVOMAX);
}

void sequence(){
  rest(); delay(500);
  pullDown(); delay(5000);
  closeUp(); delay(1000); 
}

void rest(){
  driver.setPWM(topServo, 0, convertToPulse(0));
  driver.setPWM(botServo, 0, convertToPulse(180));
}

void pullDown(){
  driver.setPWM(topServo, 0, convertToPulse(150));
  driver.setPWM(botServo, 0, convertToPulse(60));
}

void closeUp(){
  driver.setPWM(topServo, 0, convertToPulse(0));
  driver.setPWM(botServo, 0, convertToPulse(75));
} 

void setServoPulse(uint8_t n, double pulse){
  double pulseLength;
  pulseLength = 1000000/60;
  Serial.print(pulseLength); Serial.println(" us per period");
  pulseLength /= 4096;
  Serial.print(pulseLength); Serial.println(" us per bit");
  pulse *= 1000;
  pulse /= pulseLength;
  Serial.println(pulse);
  driver.setPWM(n, 0, pulse);
}
