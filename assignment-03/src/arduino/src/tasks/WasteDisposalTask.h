#ifndef __WASTE_DISPOSAL_TASK__
#define __WASTE_DISPOSAL_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/UserPanel.h"

class WasteDisposalTask: public Task {

public:
  WasteDisposalTask(WindowController* pController, UserPanel* pPanel); 
  void tick();

private:  
  enum State{ READY, DOOR_OPENING, SPILLING, DOOR_CLOSING, WASTE_RECEIVED, FULL, SLEEPING, MAINTENANCE} state;

  void setState(State state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  long stateTimestamp;
  bool justEntered;

  WindowController* pController;
  UserPanel* pPanel;
};

#endif