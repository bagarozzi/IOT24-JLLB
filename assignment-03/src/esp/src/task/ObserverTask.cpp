#include "ObserverTask.h"
#include <ArduinoJson.h>

ObserverTask::ObserverTask(SmartTemperatureSensor *sensor, MQTT_agent *agent) : sensor(sensor), agent(agent){}

void ObserverTask::tick()
{
    switch (this->getState())
    {
    case IDLE:
        while(!agent->isConnected())
        {
            sensor->setLedsToError();
            agent->reconect();
        }
        sensor->setLedsToNormal();
        this->setState(COMPUTING);
        break;
    case COMPUTING:
        if (agent->isMessageArrived())
        {
            String msg = agent->reciveMessage();
            JsonDocument doc;
            deserializeJson(doc, msg);
            sensor->setFrequency(doc["frequency"]);
        }
        this->setState(IDLE);
        break;
    }
}