#include "MainTask.h"
#include "Arduino.h"
#include "config.h"
#include <avr/sleep.h>
#include "kernel/Logger.h"
#include "kernel/MessageService.h"

MainTask::MainTask(SmartWasteBin* wasteBin, DisplayService* displayService, UserDetectionTask* userDetectionTask, MaintenanceTask* maintenanceTask) :
    wasteBin(wasteBin), displayService(displayService), userDetectionTask(userDetectionTask), maintenanceTask(maintenanceTask) {
    setState(WAITING_FOR_USER);
}

void MainTask::tick() {
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance() && this->state != IN_MAINTENANCE) {
        displayService->displayHighTemperatureMessage();
        setState(INIZIALIZE_MAINTENANCE);
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
            wasteBin->enableSleepInterrpt();
            // TODO: user console prepare sleep
            delay(100);
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_mode();
            sleep_disable();
            wasteBin->disableSleepInterrpt();
            // TODO: user console wake up
            setState(USER_DETECTED);
            break;
        case USER_DETECTED:
            logOnce(F("[main]: User detected"));
            // tasto open setta a ReadyToOpen il Bidone
            if(wasteBin->isReadyToOpen()) {
                // TODO: user console ready to open
                setState(BIN_OPENING);
            }
            else if(wasteBin->isUserGone()) {
                // TODO: user console user gone
                setState(WAITING_FOR_USER);
            }
            break;
        case BIN_OPENING:
            logOnce(F("[main]: Opening bin"));
            if(wasteBin->isBinOpen()) {
                // TODO: logOnce("Bin open");
                wasteBin->readyForDisposal();
                setState(DISPOSING);
            }
            break;
        case DISPOSING:
            logOnce(F("[main]: Disposing"));
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
            logOnce(F("[main]: Initialize maintenance"));
            maintenanceTask->setActive(true);
            userDetectionTask->setActive(false);
            setState(IN_MAINTENANCE);
            break;
        case IN_MAINTENANCE: //TODO: gestire chiusura bin
            logOnce(F("[main]: In maintenance"));
            if(wasteBin->isMaintenanceCompleted()) {
                maintenanceTask->setActive(false);
                wasteBin->setIdle();
                setState(WAITING_FOR_USER);
            }
            break;
        default:
            // Handle unknown state
            Serial.println("Unknown state🦶");
            break;
    }

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

