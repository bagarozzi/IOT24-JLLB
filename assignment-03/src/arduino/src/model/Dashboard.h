#ifndef __DASHBOARD__
#define __DASHBOARD__

#include "config.h"
#include "HWPlatform.h"
#include "model/WindowController.h"

class Dashboard {

public:
  Dashboard(WindowController* pController);

  void init();
  
  void notifyNewState();
  
  bool checkAndResetDischargeRequest();
  bool checkAndResetMaintenanceDone();

  void sync();

 private:
  WindowController* pController;
  bool dischargeCmdRequested;
  bool maintenanceDoneNotified;
  float currentTemp;
};

#endif
