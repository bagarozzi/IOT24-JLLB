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

            if (doc["state"] == "hot")
            {
                sensor->setState(1);
            }
            else if (doc["state"] == "too-hot")
            {
                sensor->setState(2);
            }
            else if (doc["state"] == "alarm")
            {
                sensor->setState(3);
            }
        }
        this->setState(IDLE);
        break;
    }
}