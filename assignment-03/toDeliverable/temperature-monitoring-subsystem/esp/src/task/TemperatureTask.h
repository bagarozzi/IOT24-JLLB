#ifndef __TEMPERATURE_TASK__
#define __TEMPERATURE_TASK__

#include "core/Task.h"
#include "core/MQTT_Agent.h"
#include "TempSensorLM35.h"
#include "model/SmartTemperatureSensor.h"
#include "core/TaskQueue.h"
class TemperatureTask : public Task
{
private:
    SmartTemperatureSensor* sensor;
    MQTT_agent* agent;
    TaskQueue<int>* frequencyQueue;
    int frequency;

    enum State{
            IDLE,
            SENDIG,
            CONNECTING,
        };
    void getFrequency();

public:
    TemperatureTask(MQTT_agent* agent, SmartTemperatureSensor* sensor, TaskQueue<int>* queue);
    static void tick(void* parameter);
};

#endif