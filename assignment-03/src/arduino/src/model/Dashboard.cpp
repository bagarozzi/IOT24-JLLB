#include "Dashboard.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "model/WindowController.h"

Dashboard::Dashboard(WindowController* pController): pController(pController){
}

void Dashboard::init(){
  maintenanceDoneNotified = false;
  dischargeCmdRequested = false;
}

void Dashboard::notifyNewState(){
  String st;
  if (pController->isInManualMode()){ // manual mode
    st = "1";
  } else { // automatic mode
    st = "0";
  }
  float windowOpeningLevel = pController->getCurrentOpeningLevel();
  MsgService.sendMsg(String("cw:st:") + st + ":" + String(windowOpeningLevel).substring(0,5) + ":" +  String(currentTemp).substring(0,5));  
}

void Dashboard::sync(){
  if (MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    if (msg != NULL){
      Logger.log("Received msg: " + msg->getContent());
      if (msg->getContent() == "ok"){
        maintenanceDoneNotified = true;
      } else if (msg->getContent() == "di"){
        dischargeCmdRequested = true;
      }
      delete msg;
    }  
  }
}

bool Dashboard::checkAndResetDischargeRequest(){
  bool com = this->dischargeCmdRequested;
  dischargeCmdRequested = false;
  return com;
}

bool Dashboard::checkAndResetMaintenanceDone(){
  bool com = this->maintenanceDoneNotified;
  maintenanceDoneNotified = false;
  return com;
}
