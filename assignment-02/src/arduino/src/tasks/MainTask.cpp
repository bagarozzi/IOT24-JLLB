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
                setState(USER_DETECTED);
            }
            else if(elapsedTimeInState() > SLEEP_TIMEOUT) {
                setState(SLEEPING);
            }
            break;
        case SLEEPING:
            // logOnce("Sleeping");
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
            // Handle USER_DETECTED state
            break;
        case BIN_OPENING:
            // Handle BIN_OPENING state
            break;
        case IN_MAINTENANCE:
            // Handle IN_MAINTENANCE state
            break;
        case DISPOSING:
            // Handle DISPOSING state
            break;
        case BIN_CLOSING:
            // Handle BIN_CLOSING state
            break;
        default:
            // Handle unknown state
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
