#include "WasteDisposalTask.h"
#include "kernel/Logger.h"
#include <Arduino.h>

#define BIN_OPENING_TIME 500
#define BIN_CLOSING_TIME 500
#define DISPOSAL_TIME 15000 //15 sec

WasteDisposalTask::WasteDisposalTask(SmartWasteBin* wasteBin, DisplayService* displayService) :
    wasteBin(wasteBin), displayService(displayService) {
    setState(IDLE);
}

void WasteDisposalTask::tick() {
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance()) {
        setState(IN_MAINTENANCE);
    }
    int timeLeft;
    switch (state) {
        case IDLE:
            logOnce("[Disposal]: Waiting for user");
            displayService->synchroniseButton();
            if (displayService->isOpenButtonPressed()) {
                wasteBin->readyToOpen();
                wasteBin->openBin();
                setState(BIN_OPENING);
            }
            break;
        case BIN_OPENING:
            logOnce("[Disposal]: Opening bin");
            if (elapsedTimeInState() > BIN_OPENING_TIME) {
                wasteBin->openingCompleted(); // sets the bin state to OPEN
                setState(WAITING_FOR_DISPOSAL);
            }
            break;
        case WAITING_FOR_DISPOSAL:
            logOnce("[Disposal]: Waiting for disposal");
            if (wasteBin->isReadyForDisposal()) {
                setState(DISPOSING);
                //altre operazioni di configurazione. es: enable button interrupt for closing
            }
            break;
        case DISPOSING:
            logOnce("[Disposal]: Disposing");
            timeLeft = (DISPOSAL_TIME - elapsedTimeInState()) / 1000;
            displayService->displayClosingMessage(timeLeft);
            displayService->synchroniseButton();
            if (wasteBin->isFull()) {
                //TODO: chiudi e maintenance
                wasteBin->problemDetected();
                setState(IN_MAINTENANCE);
            } // se il tempo di disposizione è finito o se è stato premuto il bottone di chiusura
            else if (elapsedTimeInState() > DISPOSAL_TIME || displayService->isCloseButtonPressed()) {
                wasteBin->readyToClose();
                wasteBin->closeBin();
                setState(BIN_CLOSING);
            }
            break;
        case BIN_CLOSING:
            logOnce("[Disposal]: Closing bin");
            if (elapsedTimeInState() > BIN_CLOSING_TIME) {
                wasteBin->disposalCompleted();
                setState(IDLE);
            }
            break;
        case IN_MAINTENANCE:
            logOnce("[Disposal]: The bin is full, maintenance needed");
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
