#include "OperatorPanel.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"
#include "model/HWPlatform.h"

OperatorPanel::OperatorPanel(HWPlatform* pHW) {
    pLcd = pHW->getOperatorPanelLcd();
    pButton = pHW->getButton();
}

void OperatorPanel::init(){
  pLcd->init();
  pLcd->backlight();
  pLcd->noDisplay();
  turnOnDisplay();
}

void OperatorPanel::turnOnDisplay(){
  pLcd->display();
  pLcd->clear();
}

void OperatorPanel::turnOffDisplay(){
  pLcd->noDisplay();
}

void OperatorPanel::displayInfoManualMode(int openingLevel, float temperature){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print((String)"Opening Level: " + openingLevel);
  pLcd->setCursor(0,1);
  pLcd->print("Mode: MANUAL");
  pLcd->setCursor(0,2);
  pLcd->print((String)"Temperature: " + temperature);
}

void OperatorPanel::displayInfoAutomaticMode(int openingLevel){
  pLcd->clear();
  pLcd->setCursor(0, 0); 
  pLcd->print((String)"Opening Level: " + openingLevel);
  pLcd->setCursor(0,1);
  pLcd->print("Mode: AUTOMATIC");
}

bool OperatorPanel::pressedButton(){
  return buttonPressed;
}

void OperatorPanel::sync(){
  pButton->sync();
  buttonPressed = pButton->isPressed();
}



