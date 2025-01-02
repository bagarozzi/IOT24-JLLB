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

  void adjustWindowToPercentage(float openingPercentage);
  bool adjustWindowBasedOnPotentiometer();
  bool adjustWindowAutomatically();
  void stopAdjustingWindow();

  void setCurrentOpeningLevel(float openingPercentage);
  float getCurrentOpeningLevel();
  void setFutureOpeningPercentage(float openingPercentage);
  float getFutureOpeningPercentage();
  void setCurrentTemperature(float temperature);
  float getCurrentTemperature();

  void sync();

private:
  long readPotentiometer();
  int percentageToAngle(float percentage);

  HWPlatform* pHW;
  float currentOpeningPercentage;
  float futureOpeningPercentage;
  float currentTemperature;
  bool manualMode;
};

#endif
