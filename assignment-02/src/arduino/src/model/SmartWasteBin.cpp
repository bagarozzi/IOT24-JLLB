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

    Logger.log("Calibrating sensor in the waste bin...");
    pir->calibrate();

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

// Methods for disposing:

void SmartWasteBin::openBin() {
    // servo open bin
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
    // servo close bin
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

}

void SmartWasteBin::setActuatorsInMaintenance() {

}

bool SmartWasteBin::isInMaintenance() {
    return this->state == MAINTENANCE;
}

bool SmartWasteBin::isMaintenanceCompleted() {
    return false;
}

// Methods for sleeping

void SmartWasteBin::prepareForSleep() {

}

void SmartWasteBin::wakeUp() {

}