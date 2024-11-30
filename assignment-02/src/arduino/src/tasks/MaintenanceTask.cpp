#include "MaintenanceTask.h"

MaintenanceTask::MaintenanceTask(SmartWasteBin *wasteBin) {

}

void MaintenanceTask::tick() {

    switch (state) {
        case INITIALIZE_MAINTENANCE:
            logOnce("[main]: Initialize maintenance");
            wasteBin->setActuatorsInMaintenance();
            wasteBin->closeBin(); //Non c'è bisogno di attendere la chiusura perché il bin rimane in maintenance
            setState(IN_MAINTENANCE);
            break;
        case IN_MAINTENANCE: //TODO: gestire chiusura bin
            logOnce("[main]: In maintenance");
            /*if(wasteBin->isMaintenanceCompleted()) {
                setState(WAITING_FOR_USER);
            }*///cancellare metodi isMAintenanceCompleted
            if(isMaintenanceMSGArrived("resetTemperature")){
                //setState(WAITING_FOR_USER);
            } else if(isMaintenanceMSGArrived("openToEmpty")){
                wasteBin->openBinForEmptying();
                setState(IN_MAINTENANCE_FULL);
            }
            break;
        case IN_MAINTENANCE_FULL:
            if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT){
                wasteBin->closeBin(); 
                if(elapsedTimeInState() >= MAINTENANCE_TIMEOUT + BIN_CLOSING_TIME) {
                    //setState(WAITING_FOR_USER);
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
