#ifndef __WINDOW_CONTROLLING_TASK__
#define __WINDOW_CONTROLLING_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/OperatorPanel.h"
#include <model/Dashboard.h>

class WindowControllingTask: public Task {

public:
  WindowControllingTask(WindowController* pController, OperatorPanel* pPanel, Dashboard* pDashboard);

  void tick();

private:  
  enum State{ AUTOMATIC, MANUAL, WINDOW_OPENING } state;

  void checkIfModeChangedAndUpdate();
  void checkIfManualMode();
  void checkIfAutomaticMode();

  void setState(State state);
  long elapsedTimeInState();
  bool checkAndSetJustEntered();
  
  long stateTimestamp;
  bool justEntered;
  State previousState;

  WindowController* pController;
  OperatorPanel* pPanel;
  Dashboard* pDashboard;
};

#endif