#include "tasks/TelemetryTask.h"
#include "config.h"

void TelemetryTask::tick() {
    switch (state) {
        case IDLE:
            if(elapsedTimeInState() > TELEMETRY_TIMEOUT) {
                setState(SENDING);
            }
            break;
        case SENDING:
            int statusCode = 0;
            
            // MSG service send telemetry;
            setState(IDLE);
            break;
    }
}

void TelemetryTask::setState(int s) {
    state = s;
    stateTimeStamp = millis();
    justEntered = true;
}

long TelemetryTask::elapsedTimeInState() {
    return millis() - stateTimeStamp;
}