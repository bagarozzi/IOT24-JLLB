#include <Arduino.h>
#include "tasks/TemperatureCheckTask.h"
#include "config.h"
#include "kernel/Logger.h"

enum State {
    IDLE,
    CHECKING,
    PANIC
};

TemperatureCheckTask::TemperatureCheckTask(SmartWasteBin* smartWasteBin) {
    this->wasteBin = smartWasteBin;
    this->state = IDLE;
}

void TemperatureCheckTask::tick() {
    switch (state) {
        case IDLE:
            logOnce("[temp]: Idle");
            if (elapsedTimeInState() >= TEMPERATURE_CHECK_TIMEOUT) {
                setState(CHECKING);
            }
            break;
        case CHECKING:
            logOnce("[temp]: Checking temperature");
            if (wasteBin->getCurrentTemperature() > TEMPERATURE_CHECK_MAXTEMP) {
                if (elapsedTimeInState() >= TEMPERATURE_CHECK_MAXTIME) {
                    wasteBin->problemDetected();
                    setState(PANIC);
                }
            }
            else {
                setState(IDLE);
            }
            break;
        case PANIC:
            logOnce("[temp]: Temperature is too high, waste bin alerted");
            if(wasteBin->isMaintenanceCompleted()) {
                setState(IDLE);
            }
            break;
    }
}

void TemperatureCheckTask::setState(State state) {
    this->state = state;
    this->stateTimeStamp = millis();
    this->justEnteredState = true;
}

void TemperatureCheckTask::logOnce(const String& message) {
    if (justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }
}