#include "WindowController.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include <avr/sleep.h>
#include "config.h"
#include "devices/servo_motor_impl.h"
#include "kernel/Logger.h"

WindowController::WindowController(HWPlatform* pHW):pHW(pHW){
}

void WindowController::init(){
  this->reset();
  isAvail = true;
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->off();
}
  
void WindowController::sync(){
 
}

bool WindowController::sensorsCanBeUsed(){
  return !pHW->getWindowMotor()->isOn();
}

void WindowController::startOpeningWindow(){
  if (isAvail){
    pHW->getWindowMotor()->on();
    pHW->getWindowMotor()->setPosition(180);
  }
}

void WindowController::stopOpeningWindow(){
  pHW->getWindowMotor()->off();
}

void WindowController::startClosingWindow(){
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->setPosition(90);
}

void WindowController::stopClosingWindow(){
  pHW->getWindowMotor()->off();
}

void WindowController::startOpeningForDischarging(){
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->setPosition(0);
}

void WindowController::stopOpeningForDischarging(){
  pHW->getWindowMotor()->setPosition(90);
}

void WindowController::startClosingForDischarging(){
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->setPosition(0);
}

void WindowController::stopClosingForDischarging(){
  pHW->getWindowMotor()->setPosition(90);
}

void WindowController::notifyMaintenanceCompleted(){
  this->reset();
}

void WindowController::reset(){
  isAvail = true;
  pHW->getWindowMotor()->off();
}

void WindowController::setMaintenance(){
  isAvail = false;
  pHW->getWindowMotor()->off();
}
