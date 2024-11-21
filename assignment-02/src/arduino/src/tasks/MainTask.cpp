#include "MainTask.h"
#include "Arduino.h"
#include "config.h"
#include <avr/sleep.h>
#include "kernel/Logger.h"

MainTask::MainTask(SmartWasteBin* wasteBin) {
    this->wasteBin = wasteBin;
    setState(WAITING_FOR_USER);
}

void MainTask::tick() {
    //TODO: pensa se questo controllo funzione utility e le task in cui va messo
    /* Checks if the temperature made the bin be in maintenence */
    if (wasteBin->isInMaintenance()) {
        setState(IN_MAINTENANCE);
    }
    switch (state) {
        case WAITING_FOR_USER:
            logOnce(F("[main]: Waiting for user"));
            if(wasteBin->isUserDetected()) {
                // TODO: user console turn on display
                // TODO: user console welcome message
                wasteBin->userDetected();
                setState(USER_DETECTED);
            }
            else if(elapsedTimeInState() > SLEEP_TIMEOUT) {
                setState(SLEEPING);
            }
            break;
        case SLEEPING:
            logOnce(F("[main]: No user found, going to sleep"));
            wasteBin->prepareForSleep();
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
            //TODO: chiamata sample distanza per valutare la distanza, se > di un tot isUserGone ritorna true
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
                setState(BIN_CLOSING);
            }
            else if (wasteBin->isInMaintenance()) {
                // logOnce("Problem detected");
                setState(IN_MAINTENANCE);
            }
            break;
        case BIN_CLOSING:
            logOnce("[main]: Bin closing");
            if(wasteBin->isBinClosed()) {
                setState(WAITING_FOR_USER);
            }
            break;
        case IN_MAINTENANCE:
            logOnce("[main]: In maintenance");
            if(wasteBin->isMaintenanceCompleted()) {
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
