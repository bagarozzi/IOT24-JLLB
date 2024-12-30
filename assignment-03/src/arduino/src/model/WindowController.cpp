#include "WindowController.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include <avr/sleep.h>
#include "config.h"
#include "devices/servo_motor_impl.h"
#include "kernel/Logger.h"

#define MAX_ANGLE 90
#define MAX_PERCENTAGE 100

/**
 * Constructor
 * 
 * @param pHW the hardware platform
 */
WindowController::WindowController(HWPlatform* pHW):pHW(pHW){
  this->currentOpeningLevel = 0;
  this->manualMode = false;
}

/**
 * Initializes the window controller
 */
void WindowController::init(){
  pHW->getWindowMotor()->off();
}

/**
 * Checks if the window is in manual mode
 * 
 * @return true if the window is in manual mode, false otherwise
 */
bool WindowController::isInManualMode(){
  return this->manualMode;
}

/**
 * Checks if the window is in automatic mode
 * 
 * @return true if the window is in automatic mode, false otherwise
 */
bool WindowController::isInAutomaticMode(){
  return !this->manualMode;
}

/**
 * Sets the window to manual mode
 * 
 * @return true if the window is in manual mode, false otherwise
 */
void WindowController::setManualMode(){
  this->manualMode = true;
}

/**
 * Sets the window to automatic mode
 * 
 * @return true if the window is in automatic mode, false otherwise
 */
void WindowController::setAutomaticMode(){
  this->manualMode = false;
}
  
/**
 * Syncs the potentiometer
 */
void WindowController::sync(){
  pHW->getPot()->sync();
}

/**
 * Adjusts the window to a certain percentage
 * 
 * @param percentage the percentage to adjust the window to
 */
void WindowController::adjustWindowToPercentage(float openingPercentage) {
  this->currentOpeningLevel = openingPercentage;
  int angle = percentageToAngle(openingPercentage);
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->setPosition(angle);
}

/**
 * Adjusts the window based on the potentiometer
 */
void WindowController::adjustWindowBasedOnPotentiometer() {
  adjustWindowToPercentage(readPotentiometer());
}

/**
 * Stops adjusting the window
 */
void WindowController::stopAdjustingWindow(){
  pHW->getWindowMotor()->off();
}

/**
 * Gets the current opening level
 * 
 * @return the current opening level
 */
float WindowController::getCurrentOpeningLevel(){
  return this->currentOpeningLevel;
}

/**
 * Reads the potentiometer
 * 
 * @return the potentiometer value in percentage
 */
long WindowController::readPotentiometer() {
  return map(analogRead(POT_PIN), 0, 1023, 0, MAX_PERCENTAGE);
}

/**
 * Converts a percentage to an angle
 * 
 * @param percentage the percentage to convert
 * @return the angle
 */
int WindowController::percentageToAngle(float percentage) {
  if (percentage < 0) {
    percentage = 0;
  } else if (percentage > 100) {
    percentage = 100;
  }
  return (percentage * MAX_ANGLE) / 100;
}
