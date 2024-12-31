#include "Dashboard.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "model/WindowController.h"

#define AUTOMATIC 0
#define MANUAL 1

Dashboard::Dashboard(WindowController* pController): pController(pController){
}

void Dashboard::init(){
  manualCmdRequested = false;
  automaticCmdRequested = false;
}

void Dashboard::notifyNewState(){
  String st;
  if (pController->isInManualMode()){ // manual mode
    st = "1";
  } else { // automatic mode
    st = "0";
  }
  float windowOpeningLevel = pController->getCurrentOpeningLevel();
  float currentTemperature = pController->getCurrentTemperature();
  MsgService.sendMsg(String("cw:st:") + st + ":" + String(windowOpeningLevel).substring(0,5) + ":" +  String(currentTemperature).substring(0,5));  
}

void Dashboard::sync(){
  if (MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    if (msg != NULL){
      String content = msg->getContent();
      int firstColon = content.indexOf(':');
      int secondColon = content.indexOf(':', firstColon + 1);
      int thirdColon = content.indexOf(':', secondColon + 1);

      if (firstColon != -1 && secondColon != -1 && thirdColon != -1) {
        int stateCode = content.substring(0, firstColon).toInt();
        int windowLevel = content.substring(firstColon + 1, secondColon).toFloat();
        float temp = content.substring(secondColon + 1, thirdColon).toFloat();

        pController->setCurrentOpeningLevel(windowLevel);
        pController->setCurrentTemperature(temp);

        if (stateCode == AUTOMATIC && pController->isInManualMode()) { // if the state is automatic and the controller is in manual mode then switch to automatic mode
          pController->setAutomaticMode();
          automaticCmdRequested = true;
        } else if (stateCode == MANUAL && pController->isInAutomaticMode()) { // if the state is manual and the controller is in automatic mode then switch to manual mode
          pController->setManualMode();
          manualCmdRequested = true;
        }
      }
      delete msg;
    }
  }
}

bool Dashboard::checkAndResetManualRequest(){
  bool com = this->manualCmdRequested;
  manualCmdRequested = false;
  return com;
}

bool Dashboard::checkAndResetAutomaticRequest(){
  bool com = this->automaticCmdRequested;
  automaticCmdRequested = false;
  return com;
}
