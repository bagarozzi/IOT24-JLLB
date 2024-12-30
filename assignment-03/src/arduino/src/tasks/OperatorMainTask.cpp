#include "tasks/OperatorMainTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define DISCHARGE_TIME 5000
#define OPENING_DISCHARGING_TIME 1000
#define CLOSING_DISCHARGING_TIME 1000

OperatorMainTask::OperatorMainTask(WindowController* pController, Dashboard* pDashboard): 
    pController(pController), pDashboard(pDashboard){
    setState(AUTOMATIC);
}
  
void OperatorMainTask::tick(){
    pDashboard->sync();
    pDashboard->notifyNewState();
    switch (state){    
        case AUTOMATIC: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[OM] automatic"));
                
            }
            
            break;
        }
        case MANUAL: {
            if (this->checkAndSetJustEntered()){
                Logger.log(F("[OM] manual"));
            }
            pController->adjustWindowBasedOnPotentiometer();
            break;
        }
    }
}


void OperatorMainTask::setState(State s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long OperatorMainTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool OperatorMainTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
