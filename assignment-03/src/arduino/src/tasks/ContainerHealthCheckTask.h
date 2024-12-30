#ifndef __CONTAINER_HEALTH_CHECK_TASK__
#define __CONTAINER_HEALTH_CHECK_TASK__

#include "kernel/Task.h"
#include "model/WindowController.h"
#include "model/UserPanel.h"

class ContainerHealthCheckTask: public Task {

public:
  ContainerHealthCheckTask(WindowController* pController); 
  void tick();

private:  
  void setState(int state);
  long elapsedTimeInState();
  void log(const String& msg);
  
  bool checkAndSetJustEntered();
  
  enum { NORMAL, PREALARM, WAITING_MAINTENANCE} state;
  long stateTimestamp;
  bool justEntered;

  WindowController* pController;
};

#endif