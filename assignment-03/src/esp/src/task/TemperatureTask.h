#ifndef __TEMPERATURE_TASK__
#define __TEMPERATURE_TASK__

#include "core/Task.h"
#include "core/MQTT_Agent.h"
#include "TempSensorLM35.h"
#include "model/SmartTemperatureSensor.h"
class TemperatureTask : public Task
{
private:
    SmartTemperatureSensor* sensor;
    MQTT_agent* agent;
    int frequency;

    enum State{
            IDLE,
            SENDIG,
            ALARM,
        };
public:
    TemperatureTask(MQTT_agent* agent, SmartTemperatureSensor* sensor);
    void tick();
    void setFrequency(int freq);
};

#endif