#ifndef __WINDOW_CONTROLLER__
#define __WINDOW_CONTROLLER__

#include "config.h"
#include "model/HWPlatform.h"

class WindowController {

public:
  WindowController(HWPlatform* hw);
  void init();

  bool isInManualMode();
  bool isInAutomaticMode();
  void setManualMode();
  void setAutomaticMode();

  void adjustWindowToPercentage(int openingPercentage);
  bool adjustWindowBasedOnPotentiometer();
  bool adjustWindowAutomatically();
  void stopAdjustingWindow();

  void setCurrentOpeningPercentage(int openingPercentage);
  int getCurrentOpeningPercentage();
  void setFutureOpeningPercentage(int openingPercentage);
  int getFutureOpeningPercentage();
  void setCurrentTemperature(float temperature);
  float getCurrentTemperature();

  bool checkAndResetAngleCmdRequeste();

  void sync();

private:
  long readPotentiometer();
  int percentageToAngle(int percentage);
  int checkAndFixPercentage(int percentage);

  HWPlatform* pHW;
  int currentOpeningPercentage = 1;
  int futureOpeningPercentage = 0;
  float currentTemperature = 24;
  bool manualMode = false;
  bool notifyAngleCmdRequested = false;
};

#endif
