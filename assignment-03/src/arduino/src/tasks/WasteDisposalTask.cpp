#include "WasteDisposalTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/UserPanel.h"
#include <avr/sleep.h>

#define DOOR_TIME  1000
#define SPILLING_MAX_TIME 20000 
#define WASTE_RECEIVED_TIME 2000 
#define SLEEP_TIMEOUT 120000

WasteDisposalTask::WasteDisposalTask(WindowController* pController, UserPanel* pPanel): 
    pController(pController), pPanel(pPanel) {
    setState(READY);
}
  
void WasteDisposalTask::tick(){    
    pPanel->sync();
    if (pController->isInMaintenance() && state != MAINTENANCE){
        setState(MAINTENANCE);
    }
    switch (state){    
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
    case DOOR_CLOSING: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] closing"));
            pController->startClosingDoor();
            pPanel->displayClosing();
        }
        if (elapsedTimeInState() > DOOR_TIME){
            pController->stopClosingDoor();
            setState(WASTE_RECEIVED);
        }
        break;       
    }
    case WASTE_RECEIVED: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] waste received"));
            pPanel->displayWasteReceived();
        }
        if (elapsedTimeInState() > WASTE_RECEIVED_TIME){
            if (pController->isFull()){
                setState(FULL);
            } else {
                setState(READY);
            }
        }
        break;       
    }
    case SLEEPING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] going to sleep"));
        }
        // pPlant->prepareToSleep();
        // pUserConsole->prepareToSleep();
        delay(100);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
        sleep_enable();
        sleep_mode();  
        // pPlant->resumeFromSleeping();
        // pUserConsole->resumeFromSleeping();
        setState(READY);
        break;
    }

    case FULL: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] full"));
            pPanel->displayContainerFull();
        }
        if (!pController->isFull()){
            setState(READY);
        }
        break;
    }
    case MAINTENANCE: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[WD] maintenance"));
            pPanel->displayInMaintenance();
        }    
        if (!pController->isInMaintenance()){
            setState(READY);
        }
        break;
    }
    }
}

void WasteDisposalTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long WasteDisposalTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool WasteDisposalTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
