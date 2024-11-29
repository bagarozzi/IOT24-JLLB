#include "tasks/TelemetryTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

TelemetryTask::TelemetryTask(SmartWasteBin* wasteBin) {
    this->wasteBin = wasteBin;
    setState(IDLE);
}

void TelemetryTask::tick() {
    switch (state) {
        case IDLE:
            logOnce(F("[telm]: Idle"));
            if(elapsedTimeInState() > TELEMETRY_TIMEOUT) {
                setState(SENDING);
            }
            break;
        case SENDING:
            logOnce(F("[telm]: Sending data..."));
            int statusCode = 0;
            String message = String(wasteBin->getCurrentLevel()) + ":" + String(wasteBin->getCurrentTemperature());
            // MSG service send telemetry;
            setState(IDLE);
            break;
    }
}

void TelemetryTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
    justEntered = true;
}

long TelemetryTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void TelemetryTask::logOnce(const String& message) {
    if(justEntered) {
        Logger.log(message);
        justEntered = false;
    }

}