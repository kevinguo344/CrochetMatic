#include "Motor.h"

//CONSTRUCTORS
Motor::Motor(uint8_t addr, int i, bool inv){
	driverAddress = addr;
	index = i;
	inverted = inv;
}

Motor::Motor(){}

void Motor::initializeDriver(){
  //*driver = new Adafruit_PWMServoDriver(driverAddress);
}

//SETTERS
uint8_t Motor::setDriverAddress(uint8_t a){
	driverAddress = a;
	return driverAddress;
}

int Motor::setMotorIndex(int i){
	index = i;
	return index;
}

bool Motor::setInverted(bool b){
	inverted = b;
	return inverted;
}


//GETTERS
uint8_t Motor::getDriverAddress(){
  return driverAddress;
}

int Motor::getMotorIndex(){
  return index;
}

bool Motor::getInverted(){
  return inverted;
}

