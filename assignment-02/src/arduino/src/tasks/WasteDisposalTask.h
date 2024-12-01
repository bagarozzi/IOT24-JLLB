#ifndef __WASTE_DISPOSAL_TASK__
#define __WASTE_DISPOSAL_TASK__

#include "kernel/Task.h"
#include "model/SmartWasteBin.h"
#include "model/DisplayService.h"
#include "UserDetectionTask.h"

class WasteDisposalTask : public Task {

    public:
        WasteDisposalTask(SmartWasteBin* wasteBin, DisplayService* displayService, UserDetectionTask* userDetectionTask);
        void tick();

    private:

        enum State {
            IDLE,
            BIN_OPENING,
            WAITING_FOR_DISPOSAL,
            DISPOSING,
            BIN_CLOSING,
            IN_MAINTENANCE
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);
        // TODO: LogOnce() method

        SmartWasteBin* wasteBin;
        DisplayService* displayService;
        UserDetectionTask* userDetectionTask;

        long stateTimeStamp;
        bool justEnteredState;
};

#endif