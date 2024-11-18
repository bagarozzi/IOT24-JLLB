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
 * @author Federico Bagattoni
 */
class TelemetryTask : public Task {
    
    public:
        TelemetryTask();
        void tick();

    private:
        void setState(int state);
        long elapsedTimeInState();
        // TODO: void logOnce
        
        enum {
            IDLE,
            SENDING
        } state;
        long stateTimeStamp;
        bool justEntered;

        SmartWasteBin* wasteBin;
        // userconsole* uCons;
};

#endif