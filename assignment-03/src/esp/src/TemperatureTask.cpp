#include <TemperatureTask.h>

TemperatureTask::TemperatureTask(MQTT_agent agent, SmartTemperatureSensor sensor) : agent(agent), temperatureSensor(sensor)
{
}

void TemperatureTask::tick()
{
    switch (this->getState())
    {
    case NORMAL:
        this->logOnce("normal state");
        if (this->elapsedTimeInState() >= 1000)
        {
            agent.sendMessage("");
            if (temperatureSensor.isInHotState())
            {
                this->setState(FAST);
            }
        }
        break;
    case FAST:
        this->logOnce("fast state");
        if (this->elapsedTimeInState() >= 500)
        {
            agent.sendMessage("");
            if (temperatureSensor.isInHotState())
            {
                this->setState(FAST);
            }
        }
        break;
    case ALARM:
        this->logOnce("Alarm state");
        if (temperatureSensor.isInHotState())
        {
            this->setState(FAST);
        } 
        else 
        {
            this->setState(NORMAL);
        }
        
        break;
    }
}