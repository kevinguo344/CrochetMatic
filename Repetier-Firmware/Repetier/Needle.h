#include "Repetier.h"
#include <Wire.h>
#include "Motor.h"

#define SERVOMIN 150
#define SERVOMAX 600

class Needle{
	public:
		Needle(Motor& t, Motor& b);
    //Needle(int i);
		void initializeDrivers();
		void rest();
		void closeUp();
		void pullDown();
		void sequence();
	private:
		Motor *topMotor;
		Motor *bottomMotor;
};


