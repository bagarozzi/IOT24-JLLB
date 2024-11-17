#include "MainTask.h"
#include "Arduino.h"
#include "config.h"
#include <avr/sleep.h>

MainTask::MainTask() {
    wasteBin = new SmartWasteBin();
    setState(WAITING_FOR_USER);
}

void MainTask::tick() {
    switch (state) {
        case WAITING_FOR_USER:
            // logOnce("Waiting for user");
            if(wasteBin->samplePresence()) {
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
            // TODO: logOnce("Sleeping");
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
            // TODO: logOnce("User detected");
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
            // TODO: logOnce("Bin opening");
            if(wasteBin->isBinOpen()) {
                // TODO: logOnce("Bin open");
                setState(DISPOSING);
            }
            break;
        case DISPOSING:
            // logOnce("Disposing...");
            if(wasteBin->isDisposingDone()) {
                // logOnce("Disposing done");
                setState(BIN_CLOSING);
            }
            else if (wasteBin->isInMaintenance()) {
                // logOnce("Problem detected");
                setState(IN_MAINTENANCE);
            }
            break;
        case IN_MAINTENANCE:
            if(wasteBin->isMaintenanceCompleted()) {
                setState(WAITING_FOR_USER);
            }
            break;
        case BIN_CLOSING:
            // TODO: logOnce("Bin closing");
            if(wasteBin->isBinOpen()) {
                // TODO: logOnce("Bin closed");
                setState(WAITING_FOR_USER);
            }
            break;
        default:
            // Handle unknown state
            Serial.println("Unknown state🦶");
            break;
    }

}

void MainTask::setState(int s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long MainTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}
