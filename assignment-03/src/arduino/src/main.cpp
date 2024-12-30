/*
 * ASSIGNMENT #3
 * 
 * Author:
 * Bagattoni Federico
 * Ponseggi Luca
 * Turchi Jacopo
 * Venturini Luca
 * 
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
#include "tasks/ContainerHealthCheckTask.h"
#include "tasks/OperatorManTask.h"

// #define __TESTING_HW__

Scheduler sched;

HWPlatform* pHWPlatform;
UserPanel* pUserPanel;
WindowController* pWasteContainer;
Dashboard* pDashboard; 

void setup() {
  MsgService.init();
  sched.init(100);

  Logger.log(":::::: Smart Waste Disposal ::::::");
  
  pHWPlatform = new HWPlatform();
  pHWPlatform->init();

#ifndef __TESTING_HW__
  pUserPanel = new UserPanel(pHWPlatform);
  pUserPanel->init();

  pWasteContainer = new WindowController(pHWPlatform);
  pWasteContainer->init();

  pDashboard = new Dashboard(pWasteContainer);
  pDashboard->init();

  Task* pWasteDisposalTask = new WasteDisposalTask(pWasteContainer, pUserPanel);
  pWasteDisposalTask->init(50);

  Task* pControllerHealthCheckTask = new ContainerHealthCheckTask(pWasteContainer);
  pControllerHealthCheckTask->init(100);

  Task* pOperatorManTask = new OperatorManTask(pWasteContainer, pDashboard);
  pOperatorManTask->init(200);

  sched.addTask(pControllerHealthCheckTask);
  sched.addTask(pWasteDisposalTask);
  sched.addTask(pOperatorManTask);
#endif

#ifdef __TESTING_HW__
  /* Testing */
  Task* pTestHWTask = new TestHWTask(pHWPlatform);
  pTestHWTask->init(1000);
  sched.addTask(pTestHWTask);
#endif
}

void loop() {
    sched.schedule();
}