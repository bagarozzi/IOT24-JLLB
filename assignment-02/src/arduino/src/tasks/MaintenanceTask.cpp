#include "MaintenanceTask.h"

MaintenanceTask::MaintenanceTask(SmartWasteBin *wasteBin) {
    this->wasteBin = wasteBin;
    setState(INITIALIZE_MAINTENANCE);
}

void MaintenanceTask::tick() {
    switch (state) {
        case INITIALIZE_MAINTENANCE:
            if(justEnteredState) {
                wasteBin->setActuatorsInMaintenance();
                wasteBin->closeBin();
            }
            logOnce(F("[maintenance]: Initialize maintenance"));
            if(elapsedTimeInState() >= BIN_CLOSING_TIME) {
                setState(IN_MAINTENANCE);
            }
            break;
        case IN_MAINTENANCE: //TODO: gestire chiusura bin
            logOnce(F("[maintenance]: In maintenance"));
            if(MSGService.isMessageAvailable()){
                if(isMaintenanceMSGArrived("resetTemperature")){
                    wasteBin->maintenanceCompleted();
                    setState(INITIALIZE_MAINTENANCE);
                } else {
                    wasteBin->openBinForEmptying();
                    setState(IN_MAINTENANCE_FULL);
                }
            }
            break;
        case IN_MAINTENANCE_FULL:
            logOnce(F("[maintenance]: In maintenance full"));
            if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT){
                wasteBin->closeBin(); 
                if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT + BIN_CLOSING_TIME) {
                    setState(INITIALIZE_MAINTENANCE);
                    wasteBin->maintenanceCompleted();
                }
            }
            break;
    }
}

bool MaintenanceTask::isMaintenanceMSGArrived(String message){
    if(MSGService.isMessageAvailable()){
        return MSGService.receiveMessage() == message;
    }
    return false;
}

void MaintenanceTask::setState(State s) {
    state = s;
    stateTimeStamp = millis();
    justEnteredState = true;
}

long MaintenanceTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}

void MaintenanceTask::logOnce(const String& message) {
    if(justEnteredState) {
        Logger.log(message);
        justEnteredState = false;
    }
}
