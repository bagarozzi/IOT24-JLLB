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
  void adjustWindowBasedOnPotentiometer();
  void stopAdjustingWindow();
  float getCurrentOpeningLevel();

  void sync();

private:
  long readPotentiometer();
  int percentageToAngle(float percentage);

  HWPlatform* pHW;
  float currentOpeningLevel;
  bool manualMode;
};

#endif
