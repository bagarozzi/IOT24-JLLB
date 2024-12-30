/*
 * ASSIGNMENT #3
 *
 * Author:
 * Bagattoni Federico
 * Ponseggi Luca
 * Turchi Jacopo
 * Venturini Luca
 *
 */
#include <Arduino.h>
#include "config.h"
#include "kernel/Scheduler.h"
#include "kernel/Logger.h"
#include "kernel/MsgService.h"
#include "model/HWPlatform.h"
#include "model/UserPanel.h"
#include "model/Dashboard.h"
#include "tasks/TestHWTask.h"
#include "tasks/WasteDisposalTask.h"
#include "tasks/OperatorMainTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform *pHWPlatform;
UserPanel *pUserPanel;
WindowController *pWindowController;
Dashboard *pDashboard;

void setup() {
  MsgService.init();
  sched.init(100);

  Logger.log(":::::: Window Controller Subsystem ::::::");

  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pUserPanel = new UserPanel(pHWPlatform);
  pUserPanel->init();

  pWindowController = new WindowController(pHWPlatform);
  pWindowController->init();

  pDashboard = new Dashboard(pWindowController);
  pDashboard->init();

  Task *pWasteDisposalTask = new WasteDisposalTask(pWindowController, pUserPanel);
  pWasteDisposalTask->init(50);

  Task *pOperatorMainTask = new OperatorMainTask(pWindowController, pDashboard);
  pOperatorMainTask->init(200);

  sched.addTask(pWasteDisposalTask);
  sched.addTask(pOperatorMainTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task *pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(1000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
  sched.schedule();
}