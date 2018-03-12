#include "Motor.h"

Motor::Motor(uint8_t addr, int i, bool inv){
	driverAddress = addr;
	index = i;
	inverted = inv;
}

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

/*uint8_t Motor::getDriverAddress(){
  return driverAddress;
}

uint8_t Motor::getDriverAddress(){
  return driverAddress;
}*/
