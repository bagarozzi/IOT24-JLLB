#include "WindowControllingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/OperatorPanel.h"
#include <avr/sleep.h>

#define WINDOW_TIME 200

WindowControllingTask::WindowControllingTask(WindowController* pController, OperatorPanel* pPanel): 
    pController(pController), pPanel(pPanel) {
        pController->setAutomaticMode();
        setState(AUTOMATIC);
    }
  
void WindowControllingTask::tick(){    
    pPanel->sync();
    if (pController->isInAutomaticMode() && state != AUTOMATIC){
        setState(AUTOMATIC);
    }
    if (pController->isInManualMode() && state != MANUAL){
        setState(MANUAL);
    }
    switch (state){
        case MANUAL: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[WC] manual mode"));
            }
            if (pPanel->pressedButton()) { // if the button is pressed then switch to automatic mode
                pController->setAutomaticMode();
                setState(AUTOMATIC);
            }
            if (pController->adjustWindowBasedOnPotentiometer()) { // if the window was adjusted then display the information
                pPanel->displayInfoManualMode(pController->getCurrentOpeningLevel(), pController->getCurrentTemperature());
                previousState = this->state;
                setState(WINDOW_OPENING);
            }
            break;
        }
        case AUTOMATIC: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[WC] automatic mode"));
            }
            if (pPanel->pressedButton()) { // if the button is pressed then switch to manual mode
                pController->setManualMode();
                setState(MANUAL);
            }
            if (pController->adjustWindowAutomatically()) { // if the window was adjusted then display the information
                pPanel->displayInfoAutomaticMode(pController->getCurrentOpeningLevel());
                previousState = this->state;
                setState(WINDOW_OPENING);
            }
            break;
        }
        case WINDOW_OPENING: {        
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[WC] opening"));
            }
            if (elapsedTimeInState() > WINDOW_TIME){ // waits for the window to be opened
                pController->stopAdjustingWindow();
                setState(previousState);
            }
            break;       
        }
    }
}

void WindowControllingTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long WindowControllingTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool WindowControllingTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
