#include<Needle.h>

//MOTOR FUNCTIONS
Needle::Motor(){
	driverAddress = 0x40;
	index = 0;
	inverted = false;
}

Needle::Motor(uint8_t addr, int i, bool inv){
	driverAddress = addr;
	index = i;
	inverted = inv;
}

uint8_t Needle::setDriverAddress(uint8_t a){
	driverAddress = a;
	return driverAddress;
}

int Needle::setMotorIndex(int i){
	index = i;
	return index;
}

bool Needle::setInverted(bool b){
	inverted = b;
	return inverted;
}

//NEEDLE FUNCTIONS
Needle::Needle(Motor t, Motor b){
	topMotor = t;
	bottomMotor = b;
}

void Needle::initializeDriver(){
	uint8_t *topDriver = std::find(std::begin(Needle::driverAddress), std::end(Needle::driverAddress), topMotor.driverAddress);
	if(topDriver == std::end(Needle::driverAddress)){
		driverAddress.push_back(topMotor.driverAddress);
		drivers.push_back(Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(topMotor.driverAddress));
	}
	uint8_t *bottomDriver = std::find(std::begin(Needle::driverAddress), std::end(Needle::driverAddress), bottomMotor.driverAddress);
	if(bottomDriver == std::end(Needle::driverAddress)){
		driverAddress.push_back(bottomMotor.driverAddress);
		drivers.push_back(Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(bottomMotor.driverAddress));
	}
}

void Needle::rest(){

}