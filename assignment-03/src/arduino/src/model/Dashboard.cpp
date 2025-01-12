#include "Dashboard.h"
#include <Arduino.h>
#include "kernel/MsgService.h"
#include "kernel/Logger.h"
#include "model/WindowController.h"

#define AUTOMATIC 0
#define MANUAL 1
#define NO_REQUEST -1
const String CONTAINER_PREFIX = "cw:";

Dashboard::Dashboard(WindowController* pController): pController(pController){
}

void Dashboard::init(){
  manualCmdRequested = false;
  automaticCmdRequested = false;
}

/**
 * Updates the state of the controller and sends the message if there are changes in mode or angle
 */
void Dashboard::notifyNewState(){
  int st = NO_REQUEST;
  if (pController->isInManualMode() && checkAndResetAutomaticRequest()){ // checks if the mode needs to be changed to automatic
    pController->setAutomaticMode();
    st = AUTOMATIC;
  } else if (pController->isInAutomaticMode() && checkAndResetManualRequest()) { // checks if the mode needs to be changed to manual
    pController->setManualMode();
    st = MANUAL;
  }
  if (st != NO_REQUEST) { // if the state has changed then send the message
    MsgService.sendMsg(String("cw:st:") + "mode" + ":" + String(st));
  }
  if (pController->isInManualMode() && pController->checkAndResetAngleCmdRequeste()) { // if the controller is in manual mode and the angle changed, send the current angle of the window
    int windowOpeningPercentage = pController->getCurrentOpeningPercentage();
    MsgService.sendMsg(String("cw:st:") + "angle" + ":" + String(windowOpeningPercentage).substring(0,5));
  }
}

/**
 * Syncs the dashboard with the messages
 */
void Dashboard::sync(){
  while (MsgService.isMsgAvailable()){
    processMsg();
  }
}

/**
 * Processes the incoming message
 */
void Dashboard::processMsg() {
  Msg *msg = MsgService.receiveMsg();
  if (msg != NULL) {
    String content = msg->getContent();
    if (content.startsWith(CONTAINER_PREFIX)) {
      String args = content.substring(CONTAINER_PREFIX.length());
      int firstColon = args.indexOf(':');
      if (firstColon != -1) {
        String type = args.substring(0, firstColon);
        String value = args.substring(firstColon + 1);
        if (type == "mode") {
          int stateCode = value.toInt();
          if (stateCode == AUTOMATIC && pController->isInManualMode()) { // if the state is automatic and the controller is in manual mode then switch to automatic mode
            pController->setAutomaticMode();
          }
          else if (stateCode == MANUAL && pController->isInAutomaticMode()) { // if the state is manual and the controller is in automatic mode then switch to manual mode
            pController->setManualMode();
          }
        }
        else if (type == "angle") {
          pController->setFutureOpeningPercentage(value.toInt());
        }
        else if (type == "temperature") {
          pController->setCurrentTemperature(value.toFloat());
        }
      }
    }
    delete msg;
  }
}

/**
 * Checks if the manual command was requested and resets the flag
 */
bool Dashboard::checkAndResetManualRequest(){
  bool com = this->manualCmdRequested;
  manualCmdRequested = false;
  return com;
}

/**
 * Checks if the automatic command was requested and resets the flag
 */
bool Dashboard::checkAndResetAutomaticRequest(){
  bool com = this->automaticCmdRequested;
  automaticCmdRequested = false;
  return com;
}

/**
 * Sets the manual command request
 */
void Dashboard::setManualRequest(){
  this->manualCmdRequested = true;
}

/**
 * Sets the automatic command request
 */
void Dashboard::setAutomaticRequest(){
  this->automaticCmdRequested = true;
}
