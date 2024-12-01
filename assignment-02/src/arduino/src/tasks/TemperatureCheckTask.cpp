#include <Arduino.h>
#include "TemperatureCheckTask.h"
#include "config.h"
#include "kernel/Logger.h"


TemperatureCheckTask::TemperatureCheckTask(SmartWasteBin* smartWasteBin) {
    this->wasteBin = smartWasteBin;
    setState(IDLE);
}

void TemperatureCheckTask::tick() {
    switch (state) {
        case IDLE:
            logOnce(F("[temp]: Idle"));
            if (elapsedTimeInState() >= TEMPERATURE_CHECK_TIMEOUT) {
                setState(CHECKING);
            }
            break;
        case CHECKING:
            logOnce(F("[temp]: Checking temperature"));
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
            logOnce(F("[temp]: Temperature is too high, waste bin alerted"));
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

long TemperatureCheckTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void TemperatureCheckTask::logOnce(const String& message) {
    if (justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }
}