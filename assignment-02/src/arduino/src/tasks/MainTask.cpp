#include "MainTask.h"
#include "Arduino.h"

MainTask::MainTask() {
    wasteBin = new SmartWasteBin();
    setState(WAITING_FOR_USER);
}

void MainTask::tick() {
    switch (state) {
        case WAITING_FOR_USER:
            // logOnce("Waiting for user");
            // Handle WAITING_FOR_USER state
            break;
        case SLEEPING:
            // Handle SLEEPING state
            break;
        case USER_DETECTED:
            // Handle USER_DETECTED state
            break;
        case BIN_OPENING:
            // Handle BIN_OPENING state
            break;
        case IN_MAINTENANCE:
            // Handle IN_MAINTENANCE state
            break;
        case DISPOSING:
            // Handle DISPOSING state
            break;
        case BIN_CLOSING:
            // Handle BIN_CLOSING state
            break;
        default:
            // Handle unknown state
            break;
    }

}

void MainTask::setState(int s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long MainTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}
