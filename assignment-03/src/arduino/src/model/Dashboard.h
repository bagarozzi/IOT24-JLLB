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
  
  bool checkAndResetManualRequest();
  bool checkAndResetAutomaticRequest();
  void setManualRequest();
  void setAutomaticRequest();

  void sync();

  private:
  WindowController* pController;
  bool manualCmdRequested;
  bool automaticCmdRequested;

   void processMsg();
};

#endif
