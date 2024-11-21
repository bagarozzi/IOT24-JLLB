#ifndef __WASTE_DISPOSAL_TASK__
#define __WASTE_DISPOSAL_TASK__

#include "kernel/Task.h"
#include "model/SmartWasteBin.h"

class WasteDisposalTask : public Task {

    public:
        WasteDisposalTask();
        void tick();

    private:
        void setState(int state);
        long elapsedTimeInState();
        void logOnce(const String& message);
        // TODO: LogOnce() method

        enum {
            IDLE,
            BIN_OPENING,
            WAITING_FOR_DISPOSAL,
            DISPOSING,
            BIN_CLOSING,
            IN_MAINTENANCE
        } state;

        SmartWasteBin* wasteBin;

        long stateTimeStamp;
        bool justEnteredState;
};

#endif