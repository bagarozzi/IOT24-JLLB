#ifndef __OBSERVER_TASK__
#define __OBSERVER_TASK__

#include <core/Task.h>
#include "core/MQTT_Agent.h"
#include "model/SmartTemperatureSensor.h"


class ObserverTask : public Task {
    private:
        enum {
            IDLE,
            COMPUTING,
        };

        SmartTemperatureSensor* sensor;
        MQTT_agent* agent;

    public:
        ObserverTask(SmartTemperatureSensor* sensor, MQTT_agent* agent);
        void tick();
};

#endif