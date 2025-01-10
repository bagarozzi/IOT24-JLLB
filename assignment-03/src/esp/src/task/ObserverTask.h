#ifndef __OBSERVER_TASK__
#define __OBSERVER_TASK__

#include <core/Task.h>
#include "core/MQTT_Agent.h"
#include "model/SmartTemperatureSensor.h"
#include "core/TaskQueue.h"


class ObserverTask : public Task {
    private:
        enum {
            IDLE,
            COMPUTING,
            RECONNECTING,
        };

        SmartTemperatureSensor* sensor;
        MQTT_agent* agent;
        TaskQueue<int>* frequencyQueue;

    public:
        ObserverTask(SmartTemperatureSensor* sensor, MQTT_agent* agent, TaskQueue<int>* queue);
        static void tick(void* parameter);
};

#endif