#include "Repetier.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 150
#define SERVOMAX 600

class Needle{
	public:
    Needle();
		//Needle(Motor& t, Motor& b);
		void initializeDriver();
		void rest();
		void closeUp();
		void pullDown();
		void sequence();
	private:
		//Motor topMotor;
		//Motor bottomMotor;
};


