
#include "TemperatureTask.h"

#define BASE_FREQUENCY 60000
#define HOT_FREQUENCY 30000

TemperatureTask::TemperatureTask(MQTT_agent* agent, SmartTemperatureSensor* sensor) : agent(agent), sensor(sensor)
{
    frequency = BASE_FREQUENCY;
}

void TemperatureTask::tick()
{
    switch (this->getState())
    {
    case IDLE:
        this->logOnce("[temp] : idle");
        if (this->elapsedTimeInState() >= frequency)
        {
            this->setState(SENDIG);
        }
        break;
    case SENDIG:
        this->logOnce("[temp] : sending temperature");
        agent->sendMessage((String)sensor->getTemperature());
        this->setState(IDLE);
        break;
    }
}

void TemperatureTask::setFrequency(int freq)
{
    frequency = freq;
}