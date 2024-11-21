#ifndef __TEMPERATURE_CHECK_TASK__
#define __TEMPERATURE_CHECK_TASK__

#include "kernel/Task.h"
#include <model/SmartWasteBin.h>
class TemperatureCheckTask : public Task {

    public: 

        TemperatureCheckTask(SmartWasteBin* smartWasteBin);
        void tick();

    private: 

        enum State {
            IDLE,
            CHECKING,
            PANIC
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        SmartWasteBin* wasteBin;

        long stateTimeStamp;
        bool justEnteredState;

};

#endif