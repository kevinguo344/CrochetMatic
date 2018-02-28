#include "Needle.h"
using namespace std;

//MOTOR FUNCTIONS
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

//NEEDLE FUNCTIONS
Needle::Needle(Motor t, Motor b){
  topMotor = t;
  bottomMotor = b;
}

void Needle::initializeDriver(){
  
}

void Needle::rest(){
  
}

void Needle::closeUp(){
  
}

void Needle::pullDown(){
  
}

void Needle::sequence(){
  
}

