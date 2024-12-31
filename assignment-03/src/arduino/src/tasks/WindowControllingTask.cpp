#include "WindowControllingTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/OperatorPanel.h"
#include <avr/sleep.h>

#define DOOR_TIME  1000
#define SPILLING_MAX_TIME 20000 
#define WASTE_RECEIVED_TIME 2000 
#define SLEEP_TIMEOUT 120000

WindowControllingTask::WindowControllingTask(WindowController* pController, OperatorPanel* pPanel): 
    pController(pController), pPanel(pPanel) {
    setState(READY);
}
  
void WindowControllingTask::tick(){    
    pPanel->sync();
    //if (pController->isInMaintenance() && state != MAINTENANCE){
    //    setState(MAINTENANCE);
    //}
    switch (state){    
        case MANUAL: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[WD] manual mode"));
            }
            pController->adjustWindowBasedOnPotentiometer();
            pPanel->displayInfoManualMode(pController->getCurrentOpeningLevel(), 0);
            if (pPanel->pressedButton()){
                setState(AUTOMATIC);
            }
            break;
        }
        case AUTOMATIC: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[WD] automatic mode"));
                pPanel->displayInfoAutomaticMode();
            }
            int openLevel = pPanel->getDesiredOpenLevel(); // Get the desired open level from the panel
            pController->setOpenLevel(openLevel); // Set the window opening level
            if (pPanel->pressedManual()){
                setState(MANUAL);
            }
            break;
        }
    case READY: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] ready"));
            pPanel->displayReadyToGetWaste();
        }
        if (pPanel->pressedOpen()){
            setState(DOOR_OPENING);
        } else if (elapsedTimeInState() > SLEEP_TIMEOUT && !pController->isFull()){
            setState(SLEEPING);
        }
        break;
    }
    case DOOR_OPENING: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] opening"));
            pController->startOpeningDoor();
            pPanel->displayOpening();
        }
        if (elapsedTimeInState() > DOOR_TIME){
            pController->stopOpeningDoor();
            setState(SPILLING);
        }
        break;       
    }
    case SPILLING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] spilling"));
            pPanel->displayGettingWaste();
        }
        if (pPanel->pressedClose() || 
            elapsedTimeInState() > SPILLING_MAX_TIME || 
            pController->isFull()){
            setState(DOOR_CLOSING);            
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
