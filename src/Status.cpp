#include "Status.h"

using namespace KARR;


Status Status::_instance = Status();

Status::Status() {
    _speed = 0;
    _rpm = 0;
    _fuelLevel = 0;
    _engineTemp = 0;
}

Status::~Status() {
}

Status &Status::instance() {
    return _instance;
}

const unsigned int Status::getRpm() {
    return _rpm.load();
}

const unsigned short Status::getSpeed() {
    return _speed.load();
}

const unsigned int Status::getFuelLevel() {
    return _fuelLevel.load();
}

const unsigned short Status::getEngineTemp() {
    return _engineTemp.load();
}

void Status::setRpm(unsigned int newRpm) {
    _rpm.store(newRpm);
}

void Status::setSpeed(unsigned short newSpeed) {
    _speed.store(newSpeed);
}

void Status::setFuelLevel(unsigned int newFuelLevel) {
    _fuelLevel.store(newFuelLevel);
}

void Status::setEngineTemp(unsigned short newEngineTemp) {
    _engineTemp.store(newEngineTemp);
}

