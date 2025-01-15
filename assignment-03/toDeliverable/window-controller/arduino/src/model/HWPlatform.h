#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "devices/Pot.h"
#include "devices/servo_motor.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  LiquidCrystal_I2C* getOperatorPanelLcd();
  Potentiometer* getPot();
  ServoMotor* getWindowMotor();

private:
  Button* pButton;
  LiquidCrystal_I2C* pLcd; 
  ServoMotor* pMotorWindow;
  Potentiometer* pPot;
};

#endif
