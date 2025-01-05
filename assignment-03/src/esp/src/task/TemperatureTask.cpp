
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
        if (this->elapsedTimeInState() >= frequency)
        {
            this->setState(SENDIG);
        }
        Serial.println("end");
        break;
    case SENDIG:
        this->logOnce("[TEMP] : SENDING");
        agent->sendMessage((String)sensor->getTemperature());
        this->setState(IDLE);
        break;
    }
}

void TemperatureTask::setFrequency(int freq)
{
    frequency = freq;
}