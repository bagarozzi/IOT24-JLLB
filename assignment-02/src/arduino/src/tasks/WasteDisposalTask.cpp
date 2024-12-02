#include "WasteDisposalTask.h"
#include "kernel/Logger.h"
#include <Arduino.h>

#define BIN_OPENING_TIME 500
#define BIN_CLOSING_TIME 500
#define DISPOSAL_TIME 15000 //15 seconds

WasteDisposalTask::WasteDisposalTask(SmartWasteBin* wasteBin, DisplayService* displayService, UserDetectionTask* userDetectionTask) :
    wasteBin(wasteBin), displayService(displayService), userDetectionTask(userDetectionTask) {
    setState(IDLE);
}

void WasteDisposalTask::tick() {
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance() && this->state != HIGH_TEMPERATURE_MAINTENANCE) {
        setState(HIGH_TEMPERATURE_MAINTENANCE);
    }
    int timeLeft;
    switch (state) {
        case IDLE:
            logOnce(F("[Disposal]: Waiting for user"));
            displayService->synchroniseButton();
            if (displayService->isOpenButtonPressed() && wasteBin->isUserDetected()) {
                wasteBin->readyToOpen();
                wasteBin->openBin();
                userDetectionTask->setActive(false);
                setState(BIN_OPENING);
            }
            break;
        case BIN_OPENING:
            logOnce(F("[Disposal]: Opening bin"));
            if (elapsedTimeInState() > BIN_OPENING_TIME) {
                wasteBin->openingCompleted(); // sets the bin state to OPEN
                setState(WAITING_FOR_DISPOSAL);
            }
            break;
        case WAITING_FOR_DISPOSAL:
            logOnce(F("[Disposal]: Waiting for disposal"));
            if (wasteBin->isReadyForDisposal()) {
                setState(DISPOSING);
                //altre operazioni di configurazione. es: enable button interrupt for closing
            }
            break;
        case DISPOSING:
            logOnce(F("[Disposal]: Disposing"));
            timeLeft = (DISPOSAL_TIME - elapsedTimeInState()) / 1000;
            displayService->displayClosingMessage(timeLeft);
            displayService->synchroniseButton();
            if (wasteBin->isFull()) {
                //TODO: chiudi e maintenance
                wasteBin->problemDetected();
                setState(BIN_FULL_MAINTENANCE);
            } // se il tempo di disposizione è finito o se è stato premuto il bottone di chiusura
            else if (elapsedTimeInState() > DISPOSAL_TIME || displayService->isCloseButtonPressed()) {
                wasteBin->readyToClose();
                wasteBin->closeBin();
                setState(BIN_CLOSING);
            }
            break;
        case BIN_CLOSING:
            logOnce(F("[Disposal]: Closing bin"));
            if (elapsedTimeInState() > BIN_CLOSING_TIME) {
                wasteBin->disposalCompleted();
                setState(IDLE);
            }
            break;
        case BIN_FULL_MAINTENANCE:
            logOnce(F("[Disposal]: The bin is full, maintenance needed"));
            if (wasteBin->isMaintenanceCompleted()) {
                setState(IDLE);
            }
            break;
        case HIGH_TEMPERATURE_MAINTENANCE:
            logOnce(F("[Disposal]: High temperature, maintenance needed"));
            if (wasteBin->isMaintenanceCompleted()) {
                setState(IDLE);
            }
            break;
    }

}

void WasteDisposalTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long WasteDisposalTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void WasteDisposalTask::logOnce(const String& message) {
    if(justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }
}
