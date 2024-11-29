#include <Arduino.h>
#include "model/SmartWasteBin.h"
#include "kernel/Scheduler.h"
#include "config.h"
#include "kernel/MessageService.h"

Scheduler scheduler;

SmartWasteBin* wasteBin;
// UserConsole* userConsole;

void setup() {
  MSGService.init();
  
  scheduler.init(BASE_PERIOD);
  
  wasteBin = new SmartWasteBin();
  wasteBin->init();

  // userConsoler = new UserConsole();
  // userConsole->init();

  /* Initialize and add to the scheduler all the tasks: */
}

void loop() {
  scheduler.schedule();
}