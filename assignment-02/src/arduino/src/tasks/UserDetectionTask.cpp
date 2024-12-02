#include "UserDetectionTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "kernel/MessageService.h"

UserDetectionTask::UserDetectionTask(SmartWasteBin* wasteBin) {
    this->wasteBin = wasteBin;
    setState(IDLE);
}

void UserDetectionTask::tick() {
    switch (state) {
        case IDLE:
            logOnce(F("[Detection]: Idle"));
            if(elapsedTimeInState() > DETECTION_TIMEOUT) {
                setState(CHECKING);
            }
            break;
        case CHECKING:
            logOnce(F("[Detection]: Checking for user"));
            if (wasteBin->sampleUserPresence()) {
                wasteBin->userDetected();
            }
            else {
                wasteBin->userGone();
            }
            setState(IDLE);
            break;
    }
}

void UserDetectionTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long UserDetectionTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void UserDetectionTask::logOnce(const String& message) {
    if(justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }

}