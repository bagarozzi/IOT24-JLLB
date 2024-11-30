#ifndef __USER_DETECTION_TASK__
#define __USER_DETECTION_TASK__

#include "kernel/Task.h"
#include <model/SmartWasteBin.h>

/**
 * The user detection task is responsible for detecting the presence of a user
 * in front of the waste bin.
 * It sends a message to the Java application when a user is detected.
 * 
 */
class UserDetectionTask : public Task {

    public: 

        UserDetectionTask(SmartWasteBin* smartWasteBin);
        void tick();

    private: 

        enum State {
            IDLE,
            CHECKING
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);

        SmartWasteBin* wasteBin;

        long stateTimeStamp;
        bool justEnteredState;

};

#endif