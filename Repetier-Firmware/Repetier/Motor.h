#include "Repetier.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class Motor{
  public:
		Motor(uint8_t addr, int i, bool inv);
		//setter
		uint8_t setDriverAddress(uint8_t a);
		int setMotorIndex(int i);
		bool setInverted(bool b);
    //getter
    uint8_t setDriverAddress();
    int getMotorIndex();
    bool isInverted();
  private:
		uint8_t driverAddress;
		int index;
		bool inverted;
		Adafruit_PWMServoDriver driver;
};
