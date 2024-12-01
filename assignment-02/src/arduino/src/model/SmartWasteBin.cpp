#include "SmartWasteBin.h"
#include "config.h"
#include <kernel/Logger.h>

SmartWasteBin::SmartWasteBin() {
}

void SmartWasteBin::init() {
    led1 = new Led(LED1_PIN);
    led2 = new Led(LED2_PIN);
    sonar = new Sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN, 10000);
    pir = new Pir(PIR_PIN);
    coverMotor = new ServoMotorImpl(SERVO_PIN);
    tempSensor = new TempSensorLM35(TEMP_SENSOR_PIN);

    Logger.log("Calibrating sensor in the waste bin...");
    pir->calibrate();

    led1->switchOn();
    led2->switchOff();

    state = IDLE;
}

void SmartWasteBin::setState(State s) {
    state = s;
}

// Methods for user detection

void SmartWasteBin::userDetected() {
    setState(USER_DETECTED);
}

bool SmartWasteBin::isUserDetected() {
    return this->state == USER_DETECTED;
}

void SmartWasteBin::userGone() {
    setState(USER_GONE);
}

bool SmartWasteBin::isUserGone() {
    return this->state == USER_GONE;
}

void SmartWasteBin::readyToOpen() {
    setState(READY_TO_OPEN);
}
bool SmartWasteBin::isReadyToOpen() {
    return this->state == READY_TO_OPEN;
}

bool SmartWasteBin::isIdle() {
    return this->state == IDLE;
}

// Methods for disposing:

void SmartWasteBin::openBin() {
    this->coverMotor->on();
    this->coverMotor->setPosition(SERVO_OPEN_ANGLE);
}

void SmartWasteBin::openingCompleted() {
    setState(OPENED);
}

bool SmartWasteBin::isBinOpen() {
    return this->state == OPENED;
}

void SmartWasteBin::readyForDisposal() {
    setState(READY_FOR_DISPOSAL);
}

bool SmartWasteBin::isReadyForDisposal() {
    return this->state == READY_FOR_DISPOSAL;
}

void SmartWasteBin::disposalCompleted() {
    setState(DISPOSAL_COMPLETED);
}

bool SmartWasteBin::isDisposingDone() {
    return this->state == DISPOSAL_COMPLETED;
}

void SmartWasteBin::readyToClose() {
    setState(READY_TO_CLOSE);
}

bool SmartWasteBin::isReadyToClose() {
    return this->state == READY_TO_CLOSE;
}

void SmartWasteBin::closeBin() {
    this->coverMotor->on();
    this->coverMotor->setPosition(SERVO_CLOSE_ANGLE);
}

void SmartWasteBin::closingCompleted() {
    setState(CLOSED);
}

bool SmartWasteBin::isBinClosed() {
    return this->state == CLOSED;
}

bool SmartWasteBin::isFull() {
    return this->getCurrentLevel() > MAX_LEVEL;
}

// Methods for maintenance:

void SmartWasteBin::problemDetected() {
    setState(MAINTENANCE);
}

void SmartWasteBin::setActuatorsInMaintenance() {
    led1->switchOff();
    led2->switchOn();
}

bool SmartWasteBin::isInMaintenance() {
    return this->state == MAINTENANCE;
}

bool SmartWasteBin::isMaintenanceCompleted() {
    return false;
}

void SmartWasteBin::openBinForEmptying() {
    this->coverMotor->on();
    this->coverMotor->setPosition(SERVO_MAINTENANCE_ANGLE);
}

// Methods for sleeping
static void wakeUp() {

}

void SmartWasteBin::enableSleepInterrpt() {
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), wakeUp, RISING);
}

void SmartWasteBin::disableSleepInterrpt() {
    detachInterrupt(digitalPinToInterrupt(PIR_PIN));
}

// Methods for interacting with sensors and actuators: 

double SmartWasteBin::getCurrentLevel() {
    return sonar->getDistance();
}

double SmartWasteBin::getCurrentTemperature() {
    return tempSensor->getTemperature();
}

bool SmartWasteBin::sampleUserPresence() {
    pir->sync();
    return pir->isDetected();
}

void SmartWasteBin::maintenanceCompleted() {
    setState(MAINTENANCE_COMPLETED);
}

void SmartWasteBin::setIdle() {
    setState(IDLE);
}
