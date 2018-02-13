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
  rest();
}

void loop() {
  //rest();
  sequence();
  //spin();
}

long convertToSeconds(int a){
  return map(a, 0, 180, SERVOMIN, SERVOMAX);
}

void sequence(){
  rest(); delay(500);
  pullDown(); delay(5000);
  closeUp(); delay(1000); 
}

void rest(){
  setServoPulse(topServo, convertToSeconds(0));
  setServoPulse(botServo, convertToSeconds(180));
}

void spin(){
  for(int i=0; i<180; i+=1){
    setServoPulse(topServo, convertToSeconds(i));
    setServoPulse(botServo, convertToSeconds(180-i));
    delay(15);
  }

  for(int i=179; i>=0; i-=1){
    setServoPulse(topServo, convertToSeconds(i));
    setServoPulse(botServo, convertToSeconds(180-i));
    delay(15);
  }
}

void pullDown(){
  setServoPulse(topServo, convertToSeconds(150));
  setServoPulse(botServo, convertToSeconds(60));
}

void closeUp(){
  setServoPulse(topServo, convertToSeconds(0));
  setServoPulse(botServo, convertToSeconds(75));
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  driver.setPWM(n, 0, pulse);
}
