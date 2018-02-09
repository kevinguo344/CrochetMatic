Servo topServo;
Servo botServo;

void setup(){
  topServo.attach(D0);
  botServo.attach(D1);
  rest();
}

//upper limit: top = 150 degrees, bottom = 5 degrees
//starting: top = 0 degrees, bottom = 180 degrees


//topServo.write(150); botServo.write(60); kinda works

void loop(){
  //rest();
  sequence();
}

void sequence(){
  rest();
  delay(500);
  pullDown();
  delay(5000);
  close();
  delay(500);
}

void rest(){
  topServo.write(0);
  botServo.write(180);
}

void pullDown(){
  topServo.write(150);
  botServo.write(60);
}

void close(){
  topServo.write(0);
  botServo.write(5);
}
