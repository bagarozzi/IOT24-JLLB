#ifndef __TELEMETRYTASK__
#define __TELEMETRYTASK__

#include "kernel/Task.h"
#include "config.h"
#include <model/SmartWasteBin.h>

/**
 * The telemetry task is responsible for sending telemetry data, gathered
 * from the sensors, to the Java application.
 * @author Federico Bagattoni
 */
class TelemetryTask : public Task {
    
    public:
        TelemetryTask();
        void tick();

    private:
        void setState(int state);
        // TODO: void logOnce
        
        enum {
            IDLE,
            SENDING
        } state;

        SmartWasteBin* wasteBin;
        // userconsole* uCons;
};

#endif