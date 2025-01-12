#include "tasks/MessageTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

MessageTask::MessageTask(WindowController* pController, Dashboard* pDashboard): 
    pController(pController), pDashboard(pDashboard) {
    setState(AUTOMATIC);
}
  
void MessageTask::tick() {
    pDashboard->sync();
    pDashboard->notifyNewState();
    switch (state){
        case AUTOMATIC: {
            if (this->checkAndSetJustEntered()) {
                Logger.log(F("[M] automatic"));
            }
            if (pController->isInManualMode()) {
                setState(MANUAL);
            }
            break;
        }
        case MANUAL: {
            if (this->checkAndSetJustEntered()) {
                Logger.log(F("[M] manual"));
            }
            if (pController->isInAutomaticMode()) {
                setState(AUTOMATIC);
            }
            break;
        }
    }
}

void MessageTask::setState(State s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long MessageTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool MessageTask::checkAndSetJustEntered() {
    bool entered = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return entered;
}
