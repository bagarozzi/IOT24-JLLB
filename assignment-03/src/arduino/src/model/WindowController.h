#ifndef __WINDOW_CONTROLLER__
#define __WINDOW_CONTROLLER__

#include "config.h"
#include "model/HWPlatform.h"

class WindowController {

public:
  WindowController(HWPlatform* hw);
  void init();

  void adjustWindowToPercentage(int percentage);
  void adjustWindowBasedOnPotentiometer();
  void stopAdjustingWindow();

  void sync();

private:
  long readPotentiometer();
  int percentageToAngle(int percentage);

  HWPlatform* pHW;
};

#endif
