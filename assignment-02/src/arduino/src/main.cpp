#include <Arduino.h>
#include "model/SmartWasteBin.h"
#include "model/DisplayService.h"
#include "kernel/Scheduler.h"
#include "config.h"

Scheduler scheduler;

SmartWasteBin* wasteBin;
DisplayService* displayService;

void setup() {
  scheduler.init(BASE_PERIOD);
  
  wasteBin = new SmartWasteBin();
  wasteBin->init();

  displayService = new DisplayService();
  displayService->init();

  /* Initialize and add to the scheduler all the tasks: */
}

void loop() {
  scheduler.schedule();
}