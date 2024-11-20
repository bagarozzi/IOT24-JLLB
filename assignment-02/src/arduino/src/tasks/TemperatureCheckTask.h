#ifndef __TEMPERATURE_CHECK_TASK__
#define __TEMPERATURE_CHECK_TASK__

#include "kernel/Task.h"
#include <model/SmartWasteBin.h>
class TemperatureCheckTask : public Task {

    public: 

        TemperatureCheckTask(SmartWasteBin* smartWasteBin);
        void tick();

    private: 

        void setState(int state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        enum {
            CHECKING, 
            IDLE
        } state;

        SmartWasteBin* wasteBin;

        long stateTimeStamp;
        bool justEnteredState;


};

#endif