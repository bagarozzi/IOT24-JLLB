#include "tasks/TelemetryTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "kernel/MessageService.h"

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
            if (wasteBin->isReadyToOpen() || wasteBin->isBinOpen() || wasteBin->isReadyForDisposal() || wasteBin->isDisposingDone() || wasteBin->isReadyToClose() || wasteBin->isBinClosed()) {
                statusCode = 0;
            }
            else if (wasteBin->isInMaintenance()) {
                statusCode = 1;
            }
            else if (wasteBin->isUserDetected() || wasteBin->isUserGone()) {
                statusCode = 2;
            }
            else if (wasteBin->isIdle()){
                statusCode = 3;
            }
            String message = String(statusCode) + ":" + String(wasteBin->getCurrentLevel()) + ":" + String(wasteBin->getCurrentTemperature());
            //MSGService.sendMessage(message);
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