#ifndef __TELEMETRYTASK__
#define __TELEMETRYTASK__

#include "kernel/Task.h"
#include "config.h"
#include <model/SmartWasteBin.h>

/**
 * The telemetry task is responsible for sending telemetry data, gathered
 * from the sensors, to the Java application.
 * It sends parameters separated by semicolons, in the following format:
 * 
 *                     [statuscode]:[level]:[temperature]
 * 
 */
class TelemetryTask : public Task {
    
    public:
        TelemetryTask(SmartWasteBin* wasteBin);
        void tick();

    private:

        enum State {
            IDLE,
            SENDING
        } state;

        void setState(State state);
        long elapsedTimeInState();
        void logOnce(const String& message);
    
        long stateTimeStamp;
        bool justEntered;

        SmartWasteBin* wasteBin;
        // userconsole* uCons;
};

#endif