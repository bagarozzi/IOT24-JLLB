
#include "TemperatureTask.h"

#define BASE_FREQUENCY 6000

TemperatureTask::TemperatureTask(MQTT_agent* agent, SmartTemperatureSensor* sensor) : agent(agent), sensor(sensor)
{
    frequency = BASE_FREQUENCY;
    this->setState(IDLE);
}

void TemperatureTask::tick()
{
    switch (this->getState())
    {
    case IDLE:
        this->logOnce("[TEMP] : IDLE");
        if(sensor->isFrequecyChanged())
        {
            getFrequency();
        }
        if (this->elapsedTimeInState() >= frequency)
        {
            this->setState(SENDIG);
        }
        break;
    case SENDIG:
        this->logOnce("[TEMP] : SENDING : " + (String)sensor->getTemperature());
        agent->sendMessage((String)sensor->getTemperature());
        this->setState(IDLE);
        break;
    }
}

void TemperatureTask::getFrequency()
{
    frequency = sensor->getFrequency();
}