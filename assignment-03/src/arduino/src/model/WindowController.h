#ifndef __WINDOW_CONTROLLER__
#define __WINDOW_CONTROLLER__

#include "config.h"
#include "model/HWPlatform.h"

class WindowController {

public:
  WindowController(HWPlatform* hw);
  void init();

  void reset();
  void setMaintenance();
   
  void startOpeningWindow();
  void stopOpeningWindow();
  void startClosingWindow();
  void stopClosingWindow();
  void startOpeningForDischarging();
  void stopOpeningForDischarging();
  void startClosingForDischarging();
  void stopClosingForDischarging();

  void notifyMaintenanceCompleted();  

  void sync();

private:
  bool sensorsCanBeUsed();

  HWPlatform* pHW;

  bool isAvail; 
};

#endif
