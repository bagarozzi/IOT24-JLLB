#include "tasks/OperatorManTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define DISCHARGE_TIME 5000
#define OPENING_DISCHARGING_TIME 1000
#define CLOSING_DISCHARGING_TIME 1000

OperatorManTask::OperatorManTask(WindowController* pController, Dashboard* pDashboard): 
    pController(pController), pDashboard(pDashboard){
    setState(NORMAL);
}
  
void OperatorManTask::tick(){
    pDashboard->sync();
    pDashboard->notifyNewState();
    switch (state){    
    case NORMAL: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[OM] normal."));
        }
        if (pController->isInMaintenance()){
            setState(WAITING_FOR_MAINTENANCE);
        } else if (pController->isFull()){
            setState(WAITING_FOR_DISCHARGING);
        }
        break;
    }
    case WAITING_FOR_DISCHARGING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[OM] waiting for disch."));
        }
        if (pDashboard->checkAndResetDischargeRequest()){
            Logger.log(F("Discharging request"));
            setState(OPENING_DISCHARGING);
        }
        break;
    }
    case OPENING_DISCHARGING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[OM] open discharging."));
            pController->startOpeningForDischarging();
        }
        if (elapsedTimeInState() > OPENING_DISCHARGING_TIME){
            pController->stopOpeningForDischarging();
            setState(DISCHARGING);
        }
       break;
    }
    case DISCHARGING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[OM] discharging."));
        }
        if (elapsedTimeInState() > DISCHARGE_TIME){
            setState(CLOSING_DISCHARGING);
        }
        break;
    }
    case CLOSING_DISCHARGING: {
        if (this->checkAndSetJustEntered()){
            pController->startClosingForDischarging();
            Logger.log(F("[OM] close discharging."));
        }
        if (elapsedTimeInState() > CLOSING_DISCHARGING_TIME){
            pController->stopClosingForDischarging();
            pController->notifyDischargingCompleted();
            setState(NORMAL);
        }
        break;
    }
    case WAITING_FOR_MAINTENANCE: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("[OM] waiting for maintenance."));
        }
        if (pDashboard->checkAndResetMaintenanceDone()){
            Logger.log(F("Maintenance done"));
            pController->notifyMaintenanceCompleted();
            setState(NORMAL);
        }
        break;
    }
    }
}


void OperatorManTask::setState(int s){
    state = s;
    stateTimestamp = millis();
    justEntered = true;
}

long OperatorManTask::elapsedTimeInState(){
    return millis() - stateTimestamp;
}

bool OperatorManTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}
