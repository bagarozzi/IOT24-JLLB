#include "WindowControllingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/OperatorPanel.h"
#include <avr/sleep.h>

#define WINDOW_TIME 200

WindowControllingTask::WindowControllingTask(WindowController* pController, OperatorPanel* pPanel, Dashboard* pDashboard): 
    pController(pController), pPanel(pPanel), pDashboard(pDashboard) {
        pController->setAutomaticMode();
        setState(AUTOMATIC);
    }
  
void WindowControllingTask::tick() {    
    pPanel->sync();
    checkIfModeChangedAndUpdate();
    switch (state){
        case MANUAL: {
            if (this->checkAndSetJustEntered()) {
                Logger.log(F("[WC] manual mode"));
            }
            if (pPanel->pressedButton()) { // if the button is pressed then switch to automatic mode
                pDashboard->setAutomaticRequest();
                setState(AUTOMATIC);
            }
            if (pController->adjustWindowBasedOnPotentiometer()) { // if the window was adjusted then display the information
                previousState = this->state;
                setState(WINDOW_OPENING);
            }
            // if the temperature changes
            pPanel->displayInfoManualMode(pController->getCurrentOpeningPercentage(), pController->getCurrentTemperature());
            break;
        }
        case AUTOMATIC: {
            if (this->checkAndSetJustEntered()) {
                Logger.log(F("[WC] automatic mode"));
                pPanel->displayInfoAutomaticMode(pController->getCurrentOpeningPercentage());
            }
            if (pPanel->pressedButton()) { // if the button is pressed then switch to manual mode
                pDashboard->setManualRequest();
                setState(MANUAL);
            }
            if (pController->adjustWindowAutomatically()) { // if the window was adjusted then display the information
                pPanel->displayInfoAutomaticMode(pController->getCurrentOpeningPercentage());
                previousState = this->state;
                setState(WINDOW_OPENING);
            }
            break;
        }
        case WINDOW_OPENING: {        
            if (this->checkAndSetJustEntered()) {
                Logger.log(F("[WC] opening"));
            }
            if (elapsedTimeInState() > WINDOW_TIME) { // waits for the window to be opened
                pController->stopAdjustingWindow();
                setState(previousState);
            }
            break;       
        }
    }
}

/**
 * Checks if the mode has changed and updates the state
 */
void WindowControllingTask::checkIfModeChangedAndUpdate() {
    checkIfAutomaticMode();
    checkIfManualMode();
}

void WindowControllingTask::checkIfManualMode() {
    if (pController->isInManualMode() && state != MANUAL) {
        if (state == WINDOW_OPENING) { // if the window is opening saves to go to manual mode after the window is opened
            previousState = MANUAL;
        }
        else {
            setState(MANUAL);
        }
    }
}

void WindowControllingTask::checkIfAutomaticMode() {
    if (pController->isInAutomaticMode() && state != AUTOMATIC) {
        if (state == WINDOW_OPENING) { // if the window is opening saves to go to automatic mode after the window is opened
            previousState = AUTOMATIC;
        }
        else {
            setState(AUTOMATIC);
        }
    }
}

void WindowControllingTask::setState(State s) {
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long WindowControllingTask::elapsedTimeInState() {
    return millis() - stateTimestamp;
}

bool WindowControllingTask::checkAndSetJustEntered() {
    bool bak = justEntered;
    if (justEntered) {
      justEntered = false;
    }
    return bak;
}
