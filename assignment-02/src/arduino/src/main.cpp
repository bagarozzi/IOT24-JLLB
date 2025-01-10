/*
 * Authors:
 * Bagattoni Federico
 * Ponseggi Luca
 * Turchi Jacopo
 * Venturini Luca
 */

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
#include "tasks/UserDetectionTask.h"
#include "tasks/MaintenanceTask.h"

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

  UserDetectionTask* userDetectionTask = new UserDetectionTask(wasteBin);
  userDetectionTask->init(200);

  MaintenanceTask* maintenanceTask = new MaintenanceTask(wasteBin);
  maintenanceTask->init(100);
  maintenanceTask->setActive(false);

  MainTask* mainTask = new MainTask(wasteBin, displayService, userDetectionTask, maintenanceTask);
  mainTask->init(100);

  TelemetryTask* telemetryTask = new TelemetryTask(wasteBin);
  telemetryTask->init(300);

  TemperatureCheckTask* temperatureCheckTask = new TemperatureCheckTask(wasteBin);
  temperatureCheckTask->init(300);

  WasteDisposalTask* wasteDisposalTask = new WasteDisposalTask(wasteBin, displayService, userDetectionTask);
  wasteDisposalTask->init(100);

  scheduler.addTask(mainTask);
  scheduler.addTask(userDetectionTask);
  scheduler.addTask(maintenanceTask);
  scheduler.addTask(wasteDisposalTask);
  scheduler.addTask(telemetryTask);
  scheduler.addTask(temperatureCheckTask);
}

void loop() {
  scheduler.schedule();
}