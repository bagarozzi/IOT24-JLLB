#ifndef __OPERATOR_MAIN_TASK__
#define __OPERATOR_MAIN_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/Dashboard.h"

class OperatorMainTask: public Task {

public:
  OperatorMainTask(WindowController* pController, Dashboard* pDashboard); 
  void tick();

private:  
  enum State{ AUTOMATIC, MANUAL } state;

  void setState(State state);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();
  
  long stateTimestamp;
  bool justEntered;

  WindowController* pController;
  Dashboard* pDashboard;
};

#endif