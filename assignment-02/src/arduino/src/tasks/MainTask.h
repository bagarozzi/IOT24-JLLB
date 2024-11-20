#ifndef __MAIN_TASK__
#define __MAIN_TASK__

#include "kernel/Task.h"
#include "model/SmartWasteBin.h"

class MainTask : public Task {

    public:

        MainTask();
        void tick();

    private:

        void setState(int state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        enum {
            WAITING_FOR_USER,
            SLEEPING,
            USER_DETECTED,
            BIN_OPENING,
            IN_MAINTENANCE,
            DISPOSING,
            BIN_CLOSING
        } state;

        SmartWasteBin* wasteBin;

        long stateTimeStamp;
        bool justEnteredState;
        
};

#endif