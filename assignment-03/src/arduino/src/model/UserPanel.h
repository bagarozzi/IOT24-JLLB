#ifndef __USER_PANEL__
#define __USER_PANEL__

#include "config.h"
#include "HWPlatform.h"
#include <LiquidCrystal_I2C.h>

class UserPanel {

public:
  UserPanel(HWPlatform* pHW);

  void init();

  void turnOffDisplay();
  void turnOnDisplay();

  void displayInfoManualMode(int openingLevel, float temperature);
  void displayInfoAutomaticMode(int openingLevel);
  
  bool pressedButton();

  void sync();

 private:
  LiquidCrystal_I2C* pLcd; 
  Button* pButton;
  bool buttonPressed;

};

#endif
