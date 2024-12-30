#ifndef __OPERATOR_MAN_TASK__
#define __OPERATOR_MAN_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/Dashboard.h"

class OperatorManTask: public Task {

public:
  OperatorManTask(WindowController* pController, Dashboard* pDashboard); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum { NORMAL, WAITING_FOR_DISCHARGING, OPENING_DISCHARGING, DISCHARGING, 
         CLOSING_DISCHARGING, WAITING_FOR_MAINTENANCE } state;
  long stateTimestamp;
  bool justEntered;

  WindowController* pController;
  Dashboard* pDashboard;
};

#endif