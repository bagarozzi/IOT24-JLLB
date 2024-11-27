#include "SmartWasteBin.h"

SmartWasteBin::SmartWasteBin() {
}

void SmartWasteBin::init() {
    
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