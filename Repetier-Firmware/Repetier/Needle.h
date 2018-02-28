#include "Repetier.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN 150
#define SERVOMAX 600

//std::vector<uint8_t> driverAddress;
//std::vector<Adafruit_PWMServoDriver:Adafruit_PWMServoDriver> drivers;

class Motor{
  public:
		Motor(uint8_t addr, int i, bool inv);
		uint8_t setDriverAddress(uint8_t a);
		int setMotorIndex(int i);
		bool setInverted(bool b);
  private:
		uint8_t driverAddress;
		int index;
		bool inverted;
		Adafruit_PWMServoDriver driver;
};

class Needle{
	public:
		Needle(Motor t, Motor b);
		void initializeDriver();
		void rest();
		void closeUp();
		void pullDown();
		void sequence();
	private:
		Motor topMotor;
		Motor bottomMotor;
};


