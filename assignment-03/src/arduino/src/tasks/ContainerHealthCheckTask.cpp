#include "ContainerHealthCheckTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define MAXTEMP 100
#define MAXTEMPTIME 10000

ContainerHealthCheckTask::ContainerHealthCheckTask(WindowController* pController): 
    pController(pController){
    setState(NORMAL);
}
  
void ContainerHealthCheckTask::tick(){
    pController->sync();
    switch (state){    
    case NORMAL: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[CHC] normal"));
            pController->reset();
        }
        if (pController->getCurrentTemperature() > MAXTEMP){
            setState(PREALARM);
        }
        break;
    }
    case PREALARM: {        
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[CHC] pre-alarm"));
        }
        
        if (pController->getCurrentTemperature() < MAXTEMP){
            setState(NORMAL);
        } else if (elapsedTimeInState() > MAXTEMPTIME){
            setState(WAITING_MAINTENANCE);
        }
        break;       
    }
    case WAITING_MAINTENANCE: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[CHC] alarm"));
            pController->setMaintenance();
        }

        if (pController->isAvailable()){
            setState(NORMAL);
        }
        break;
    }    
    }
}

void ContainerHealthCheckTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long ContainerHealthCheckTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool ContainerHealthCheckTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
