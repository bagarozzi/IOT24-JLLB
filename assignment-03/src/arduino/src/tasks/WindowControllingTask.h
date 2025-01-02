#ifndef __WINDOW_CONTROLLING_TASK__
#define __WINDOW_CONTROLLING_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/OperatorPanel.h"

class WindowControllingTask: public Task {

public:
  WindowControllingTask(WindowController* pController, OperatorPanel* pPanel); 
  void tick();

private:  
  enum State{ AUTOMATIC, MANUAL, WINDOW_OPENING } state;

  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  long stateTimestamp;
  bool justEntered;
  State previousState;

  WindowController* pController;
  OperatorPanel* pPanel;
};

#endif