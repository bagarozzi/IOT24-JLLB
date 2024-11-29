#include <Arduino.h>
#include "model/SmartWasteBin.h"
#include "model/DisplayService.h"
#include "kernel/Scheduler.h"
#include "config.h"
#include "kernel/MessageService.h"

#include "tasks/MainTask.h"
#include "tasks/TemperatureCheckTask.h"
#include "tasks/WasteDisposalTask.h"
#include "tasks/TelemetryTask.h"

Scheduler scheduler;

SmartWasteBin* wasteBin;
DisplayService* displayService;

void setup() {
  MSGService.init();
  
  scheduler.init(BASE_PERIOD);
  
  wasteBin = new SmartWasteBin();
  wasteBin->init();

  displayService = new DisplayService();
  displayService->init();

  /* Initialize and add to the scheduler all the tasks: */

  MainTask* mainTask = new MainTask(wasteBin, displayService);
  mainTask->init(100);

  //TelemetryTask* telemetryTask = new TelemetryTask(wasteBin);
  //telemetryTask->init(300);

  //TemperatureCheckTask* temperatureCheckTask = new TemperatureCheckTask(wasteBin);
  //temperatureCheckTask->init(300);

  //WasteDisposalTask* wasteDisposalTask = new WasteDisposalTask(wasteBin, displayService);
  //wasteDisposalTask->init(100);

  //UserDetectionTask* userDetectionTask = new UserDetectionTask(wasteBin);
  //userDetectionTask->init(100);

  scheduler.addTask(mainTask);
}

void loop() {
  scheduler.schedule();
}