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
  if (pController->isInMaintenance()){
    st = "2";
  } else if (pController->isFull()){
    st = "1";
  } else {   
    st = "0"; /* available, not full */
  }
  int wasteLevel = pController->getWasteLevelPercentage();
  float currentTemp = pController->getCurrentTemperature();
  MsgService.sendMsg(String("cw:st:") + st + ":" + String(wasteLevel).substring(0,5) + ":" +  String(currentTemp).substring(0,5));  
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
