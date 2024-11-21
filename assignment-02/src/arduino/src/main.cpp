#include <Arduino.h>
#include "model/SmartWasteBin.h"
#include "kernel/Scheduler.h"
#include "config.h"

Scheduler scheduler;

SmartWasteBin* wasteBin = new SmartWasteBin();
// UserConsole* uCons = new UserConsoler();

void setup() {
  scheduler.init(BASE_PERIOD);
}

void loop() {
  scheduler.schedule();
}