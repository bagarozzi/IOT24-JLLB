#ifndef __MAIN_TASK__
#define __MAIN_TASK__

#include "kernel/Task.h"
#include "model/SmartWasteBin.h"
#include <model/DisplayService.h>
#include "UserDetectionTask.h"
#include "MaintenanceTask.h"

class MainTask : public Task {

    public:

        MainTask(SmartWasteBin* wasteBin, DisplayService* displayService, UserDetectionTask* userDetectionTask, MaintenanceTask* maintenanceTask);
        void tick();

    private:

        enum State {
            WAITING_FOR_USER,
            SLEEPING,
            USER_DETECTED,
            BIN_OPENING,
            INIZIALIZE_MAINTENANCE,
            IN_MAINTENANCE,
            IN_MAINTENANCE_FULL,
            DISPOSING,
            BIN_CLOSING
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        SmartWasteBin* wasteBin;
        DisplayService* displayService;
        UserDetectionTask* userDetectionTask;
        MaintenanceTask* maintenanceTask;

        long stateTimeStamp;
        bool justEnteredState;
        
};

#endif