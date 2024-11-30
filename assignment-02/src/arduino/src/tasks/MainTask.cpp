#include "MainTask.h"
#include "Arduino.h"
#include "config.h"
#include <avr/sleep.h>
#include "kernel/Logger.h"
#include "kernel/MessageService.h"

MainTask::MainTask(SmartWasteBin* wasteBin, DisplayService* displayService, UserDetectionTask* userDetectionTask) :
    wasteBin(wasteBin), displayService(displayService), userDetectionTask(userDetectionTask) {
    setState(WAITING_FOR_USER);
}

void MainTask::tick() {
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance()) {
        displayService->displayHighTemperatureMessage();
        setState(IN_MAINTENANCE);
    }
    switch (state) {
        case WAITING_FOR_USER:
            if(justEnteredState) {
                displayService->turnOnDisplay();
                displayService->displayInitialMessage();
            }
            logOnce(F("[main]: Waiting for user"));
            userDetectionTask->setActive(true);
            if(wasteBin->isUserDetected()) {
                setState(USER_DETECTED);
            }
            else if(elapsedTimeInState() > SLEEP_TIMEOUT) {
                setState(SLEEPING);
            }
            break;
        case SLEEPING:
            logOnce(F("[main]: No user found, going to sleep"));
            displayService->turnOffDisplay();
            // TODO: user console prepare sleep
            delay(100);
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_mode();
            wasteBin->wakeUp();
            // TODO: user console wake up
            setState(WAITING_FOR_USER);
            break;
        case USER_DETECTED:
            logOnce("[main]: User detected");
            // tasto open setta a ReadyToOpen il Bidone
            if(wasteBin->isReadyToOpen()) {
                // TODO: user console ready to open
                userDetectionTask->setActive(false);
                setState(BIN_OPENING);
            }
            else if(wasteBin->isUserGone()) {
                // TODO: user console user gone
                setState(WAITING_FOR_USER);
            }
            break;
        case BIN_OPENING:
            logOnce("[main]: Opening bin");
            if(wasteBin->isBinOpen()) {
                // TODO: logOnce("Bin open");
                wasteBin->readyForDisposal();
                setState(DISPOSING);
            }
            break;
        case DISPOSING:
            logOnce("[main]: Disposing");
            if(wasteBin->isDisposingDone()) {
                // logOnce("Disposing done");
                displayService->displayFinalDisposingMessage();
                if(elapsedTimeInState() > MESSAGE_CLOSING_TIMEOUT) {
                    setState(WAITING_FOR_USER);
                }
            }
            else if (wasteBin->isInMaintenance()) {
                // logOnce("Problem detected");
                displayService->displayContainerFullMessage();
                setState(INIZIALIZE_MAINTENANCE);
            }
            break;
        case INIZIALIZE_MAINTENANCE:
            logOnce("[main]: Initialize maintenance");
            wasteBin->setActuatorsInMaintenance();
            wasteBin->closeBin(); //Non c'è bisogno di attendere la chiusura perché il bin rimane in maintenance
            setState(IN_MAINTENANCE);
            break;
        case IN_MAINTENANCE: //TODO: gestire chiusura bin
            logOnce("[main]: In maintenance");
            /*if(wasteBin->isMaintenanceCompleted()) {
                setState(WAITING_FOR_USER);
            }*///cancellare metodi isMAintenanceCompleted
            if(isMaintenanceMSGArrived("resetTemperature")){
                setState(WAITING_FOR_USER);
            } else if(isMaintenanceMSGArrived("openToEmpty")){
                wasteBin->openBinForEmptying();
                setState(IN_MAINTENANCE_FULL);
            }
            break;
        case IN_MAINTENANCE_FULL:
            if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT){
                wasteBin->closeBin();
                if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT + 500) {
                    setState(WAITING_FOR_USER);
                }
            }
            break;
        default:
            // Handle unknown state
            Serial.println("Unknown state🦶");
            break;
    }

}

bool MainTask::isMaintenanceMSGArrived(String message){
    if(MSGService.isMessageAvailable()){
        return MSGService.receiveMessage() == message;
    }
    return false;
}

void MainTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long MainTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void MainTask::logOnce(const String& message) {
    if(justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }
}

