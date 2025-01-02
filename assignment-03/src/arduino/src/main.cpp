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
#include "model/OperatorPanel.h"
#include "model/Dashboard.h"
#include "tasks/TestHWTask.h"
#include "tasks/WindowControllingTask.h"
#include "tasks/OperatorMainTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform *pHWPlatform;
OperatorPanel *pOperatorPanel;
WindowController *pWindowController;
Dashboard *pDashboard;

void setup() {
  MsgService.init();
  sched.init(100);

  Logger.log(":::::: Window Controller Subsystem ::::::");

  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pOperatorPanel = new OperatorPanel(pHWPlatform);
  pOperatorPanel->init();

  pWindowController = new WindowController(pHWPlatform);
  pWindowController->init();

  pDashboard = new Dashboard(pWindowController);
  pDashboard->init();

  Task *pOperatorMainTask = new OperatorMainTask(pWindowController, pDashboard);
  pOperatorMainTask->init(100);

  Task *pWindowControllingTask = new WindowControllingTask(pWindowController, pOperatorPanel, pDashboard);
  pWindowControllingTask->init(100);

  sched.addTask(pOperatorMainTask);
  sched.addTask(pWindowControllingTask);
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