#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Pot.h"
#include "devices/servo_motor_impl.h"
#include "kernel/Logger.h"

HWPlatform::HWPlatform(){
  pLcd = new  LiquidCrystal_I2C(0x27,20,4);
  pButton = new ButtonImpl(BT_PIN);
  pPot = new Potentiometer(POT_PIN);
  pMotorWindow = new ServoMotorImpl(MOTOR_PIN);
}


void HWPlatform::init(){
  pLcd->init();
  pLcd->backlight();
}

  Button* HWPlatform::getButton(){
    return this->pButton;
  }

  LiquidCrystal_I2C* HWPlatform::getOperatorPanelLcd(){
    return this->pLcd;
  }

  Potentiometer*  HWPlatform::getPot(){
    return this->pPot;
  }

  ServoMotor* HWPlatform::getWindowMotor(){
    return this->pMotorWindow;
  }


void HWPlatform::test(){
  pPot->sync();
  pButton->sync();
  float value = pPot->getValue();
  bool buttonPressed = pButton->isPressed();
  pLcd->setCursor(0,0);
  pLcd->print("hello");
  pMotorWindow->on();
  pMotorWindow->setPosition(90);
  Logger.log("Pot: " + String(value));
  Logger.log("Button: " + String(buttonPressed ? "pressed" : " not pressed"));
  delay(1000);
  pMotorWindow->setPosition(0);
  delay(1000);
  pMotorWindow->off();
  pLcd->clear();
}

