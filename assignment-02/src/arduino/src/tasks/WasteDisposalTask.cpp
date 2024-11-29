#include "WasteDisposalTask.h"
#include "kernel/Logger.h"
#include <Arduino.h>

#define BIN_OPENING_TIME 500
#define BIN_CLOSING_TIME 500
#define DISPOSAL_TIME 15000 //15 sec

WasteDisposalTask::WasteDisposalTask(SmartWasteBin* wasteBin) {
    this->wasteBin = wasteBin;
    setState(IDLE);
}

void WasteDisposalTask::tick() {
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance()) {
        setState(IN_MAINTENANCE);
    }
    switch (state) {
        case IDLE:
            logOnce("[Disposal]: Waiting for user");
            if (wasteBin->isReadyToOpen()) {
                wasteBin->openBin();
                setState(BIN_OPENING);
            }
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
            if (wasteBin->isFull()) {
                //TODO: chiudi e maintenance
                wasteBin->problemDetected();
                setState(IN_MAINTENANCE);
            } // se il tempo di disposizione è finito o se è stato premuto il bottone di chiusura
            else if (elapsedTimeInState() > DISPOSAL_TIME || wasteBin->isReadyToClose()){
                wasteBin->disposalCompleted();
                wasteBin->closeBin();
                setState(BIN_CLOSING);
            }
            break;
        case BIN_CLOSING:
            logOnce("[Disposal]: Closing bin");
            if (elapsedTimeInState() > BIN_CLOSING_TIME) {
                wasteBin->closingCompleted();
                setState(IDLE);
            }
            break;
        case IN_MAINTENANCE:
            logOnce("[Disposal]: The bin is full, maintenance needed");
            if (wasteBin->isMaintenanceCompleted()) {
                setState(IDLE);
            }
            break;
        default:
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
