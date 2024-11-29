#ifndef __MAIN_TASK__
#define __MAIN_TASK__

#include "kernel/Task.h"
#include "model/SmartWasteBin.h"
#include <model/DisplayService.h>

class MainTask : public Task {

    public:

        MainTask(SmartWasteBin* wasteBin, DisplayService* displayService);
        void tick();

    private:

        enum State {
            WAITING_FOR_USER,
            SLEEPING,
            USER_DETECTED,
            BIN_OPENING,
            IN_MAINTENANCE,
            DISPOSING,
            BIN_CLOSING
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        SmartWasteBin* wasteBin;
        DisplayService* displayService;

        long stateTimeStamp;
        bool justEnteredState;
        
};

#endif