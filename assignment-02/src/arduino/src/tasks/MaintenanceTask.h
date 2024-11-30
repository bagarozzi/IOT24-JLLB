#ifndef __MAINTENANCETASK__
#define __MAINTENANCETASK__

#include "kernel/Task.h"
#include <model/SmartWasteBin.h>
#include "config.h"
#include <Arduino.h>
#include "kernel/Logger.h"
#include "kernel/MessageService.h"

class MaintenanceTask : public Task {
    
    public:

        MaintenanceTask(SmartWasteBin* wasteBin);
        void tick();

    private:

        enum State {
            INITIALIZE_MAINTENANCE,
            IN_MAINTENANCE,
            IN_MAINTENANCE_FULL
        } state;

        bool isMaintenanceMSGArrived(String message);
        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);
    
        long stateTimeStamp;
        bool justEnteredState;

        SmartWasteBin* wasteBin;
        // userconsole* uCons;
};

#endif