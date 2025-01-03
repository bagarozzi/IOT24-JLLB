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
  this->currentOpeningPercentage = 0;
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
void WindowController::adjustWindowToPercentage(int openingPercentage) {
  this->currentOpeningPercentage = openingPercentage;
  int angle = percentageToAngle(openingPercentage);
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
  int potentiometerValue = readPotentiometer();
  adjusted = getCurrentOpeningPercentage() != potentiometerValue;
  if (adjusted) {
    adjustWindowToPercentage(potentiometerValue);
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
  adjusted = getCurrentOpeningPercentage() != getFutureOpeningPercentage();
  if (adjusted) {
    adjustWindowToPercentage(getFutureOpeningPercentage());
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
int WindowController::percentageToAngle(int percentage) {
  if (percentage < 0) {
    percentage = 0;
  } else if (percentage > 100) {
    percentage = 100;
  }
  return (percentage * MAX_ANGLE) / 100;
}