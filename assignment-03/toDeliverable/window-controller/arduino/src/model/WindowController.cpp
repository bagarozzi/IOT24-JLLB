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
WindowController::WindowController(HWPlatform* pHW):pHW(pHW){}

/**
 * Initializes the window controller
 */
void WindowController::init(){
  this->manualMode = false;
  this->notifyAngleCmdRequested = false;
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
void WindowController::adjustWindowToPercentage(int openingPercentage) {
  this->currentOpeningPercentage = checkAndFixPercentage(openingPercentage);
  int angle = percentageToAngle(this->currentOpeningPercentage);
  pHW->getWindowMotor()->on();
  pHW->getWindowMotor()->setPosition(angle);
}

/**
 * Adjusts the window based on the potentiometer if necessary
 * 
 * @return true if the window was adjusted, false otherwise
 */
bool WindowController::adjustWindowBasedOnPotentiometer() {
  bool adjusted = false;
  int potentiometerValue = checkAndFixPercentage(readPotentiometer());
  adjusted = getCurrentOpeningPercentage() != potentiometerValue;
  if (adjusted) {
    adjustWindowToPercentage(potentiometerValue);
    notifyAngleCmdRequested = true;
  }
  return adjusted;
}

/**
 * Adjusts the window automatically based on the future opening percentage
 * 
 * @return true if the window was adjusted, false otherwise
 */
bool WindowController::adjustWindowAutomatically() {
  bool adjusted = false;
  int fixedPercentage = checkAndFixPercentage(getFutureOpeningPercentage());
  adjusted = getCurrentOpeningPercentage() != fixedPercentage;
  if (adjusted) {
    adjustWindowToPercentage(fixedPercentage);
  }
  return adjusted;
}

/**
 * Stops adjusting the window
 */
void WindowController::stopAdjustingWindow(){
  pHW->getWindowMotor()->off();
}

/**
 * Gets the current opening Percentage
 * 
 * @return the current opening Percentage
 */
int WindowController::getCurrentOpeningPercentage(){
  return this->currentOpeningPercentage;
}

/**
 * Sets the current opening Percentage
 */
void WindowController::setCurrentOpeningPercentage(int openingPercentage){
  this->currentOpeningPercentage = openingPercentage;
}

/**
 * Gets the future opening percentage
 */
int WindowController::getFutureOpeningPercentage(){
  return this->futureOpeningPercentage;
}

/**
 * Sets the future opening percentage
 */
void WindowController::setFutureOpeningPercentage(int openingPercentage){
  this->futureOpeningPercentage = openingPercentage;
}

/**
 * Sets the current temperature
 */
void WindowController::setCurrentTemperature(float temperature){
  this->currentTemperature = temperature;
}

/**
 * Gets the current temperature
 */
float WindowController::getCurrentTemperature(){
  return this->currentTemperature;
}

/**
 * Checks and resets the angle command request
 * 
 * @return true if the angle command was requested, false otherwise
 */
bool WindowController::checkAndResetAngleCmdRequeste(){
  bool com = this->notifyAngleCmdRequested;
  notifyAngleCmdRequested = false;
  return com;
}

/**
 * Reads the potentiometer
 * 
 * @return the potentiometer value in percentage
 */
long WindowController::readPotentiometer() { // 980 is the maximum value of the potentiometer
  return map(analogRead(POT_PIN), 0, 980, 0, MAX_PERCENTAGE);
}

/**
 * Converts a percentage to an angle
 * 
 * @param percentage the percentage to convert
 * @return the angle
 */
int WindowController::percentageToAngle(int percentage) {
  if (percentage < 0) {
    percentage = 0;
  } else if (percentage > 100) {
    percentage = 100;
  }
  return (percentage * MAX_ANGLE) / 100;
}

/**
 * Checks and fixes the percentage
 * 
 * @param percentage the percentage to check and fix
 * @return the fixed percentage
 */
int WindowController::checkAndFixPercentage(int percentage) {
  if (percentage < 0) {
    percentage = 0;
  } else if (percentage > 100) {
    percentage = 100;
  }
  return percentage;
}
