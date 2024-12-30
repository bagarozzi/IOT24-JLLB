#include "UserPanel.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/HWPlatform.h"

UserPanel::UserPanel(HWPlatform* pHW) {
    pLcd = pHW->getOperatorPanelLcd();
    pButton = pHW->getButton();
}

void UserPanel::init(){
  pLcd->init();
  pLcd->backlight();
  pLcd->noDisplay();
  turnOnDisplay();
}

void UserPanel::turnOnDisplay(){
  pLcd->display();
  pLcd->clear();
}

void UserPanel::turnOffDisplay(){
  pLcd->noDisplay();
}

void UserPanel::displayInfoManualMode(int openingLevel, float temperature){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print((String)"Opening Level: " + openingLevel);
  pLcd->setCursor(0,1);
  pLcd->print("Mode: MANUAL");
  pLcd->setCursor(0,2);
  pLcd->print((String)"Temperature: " + temperature);
}

void UserPanel::displayInfoAutomaticMode(int openingLevel){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print((String)"Opening Level: " + openingLevel);
  pLcd->setCursor(0,1);
  pLcd->print("Mode: AUTOMATIC");
}

bool UserPanel::pressedButton(){
  return buttonPressed;
}

void UserPanel::sync(){
  pButton->sync();
  buttonPressed = pButton->isPressed();
}



