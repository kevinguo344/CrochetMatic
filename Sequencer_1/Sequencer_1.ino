#include <Servo.h>

Servo bottomServos[5];
Servo topServos[5];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i <5; i++){
    topServos[i].attach(i+2);
    bottomServos[i].attach(i+9);
  }
  topServos[0].write(180);
  topServos[1].write(0);
  topServos[2].write(180);
  topServos[3].write(0);
  topServos[4].write(180);

  bottomServos[0].write(0);
  bottomServos[1].write(180);
  bottomServos[2].write(0);
  bottomServos[3].write(180);
  bottomServos[4].write(0);
}

void loop() {
  //restPosition();
  sequence();
}

void restPosition(){
  topServos[0].write(180);
  topServos[1].write(0);
  topServos[2].write(180);
  topServos[3].write(0);
  topServos[4].write(180);

  bottomServos[0].write(0);
  bottomServos[1].write(180);
  bottomServos[2].write(0);
  bottomServos[3].write(180);
  bottomServos[4].write(0);
}

void sequence(){
  for(int d=0;d<5;d+=1){
     needleSequence(d);
  }
  for(int d=3;d>0;d-=1){
    needleSequence(d);
  }
}

void needleSequence(int dom){
  Servo top = topServos[dom];
  Servo bottom = bottomServos[dom];
  if(dom%2==0){

    //EVEN # SEQUENCE
    //          START  PUSH UP   CLOSE     END
    //  TOP     180  ->  140  ->  140  ->  180
    //  BOTTOM  0    ->  0    ->  80   ->  0
    //push up 
    for(int a=0; a<=40; a+=1){
      top.write(180-a);
      delay(15);
    }
    Serial.print("------------NEEDLE "); Serial.print(dom); Serial.println(" SEQUENCE------------");
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(1000);
    //close
    for(int a = 0; a<=80; a+=1){
      bottom.write(a);
      delay(15);
    }
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(1000);
    //reset
    for(int a = 0; a<=40; a+=1){
      top.write(140+a);
      bottom.write(80-2*a);
      delay(15);
    }
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(5000);
  }
  else{
    //ODD # SEQUENCE
    //          START  PUSH UP  CLOSE     END
    //  TOP     0   ->  40  ->  40    ->  0
    //  BOTTOM  180 ->  180 ->  100   ->  180

    for(int a=0; a<=40; a+=1){
      top.write(a);
      delay(15);
    }
    Serial.print("------------NEEDLE "); Serial.print(dom); Serial.println(" SEQUENCE------------");
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(1000);
    //close
    for(int a = 0; a<=80; a+=1){
      bottom.write(180-a);
      delay(15);
    }
    
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(1000);
    
    //reset
    for(int a = 0; a<=40; a+=1){
      top.write(40-a);
      bottom.write(100+2*a);
      delay(15);
    }
    Serial.print("Top Needle is at ");
    Serial.println(top.read());

    Serial.print("Bottom Needle is at ");
    Serial.println(bottom.read());
    delay(5000);
  }
}
