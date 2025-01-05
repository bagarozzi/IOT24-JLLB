#include "ObserverTask.h"
#include <ArduinoJson.h>

ObserverTask::ObserverTask(SmartTemperatureSensor *sensor, MQTT_agent *agent) : sensor(sensor), agent(agent)
{
    this->setState(IDLE);
}

void ObserverTask::tick()
{
    switch (this->getState())
    {
    case IDLE:
        this->logOnce("[OBSERVER] : IDLE");
        while(!agent->isConnected())
        {
            Serial.println("connecting");
            sensor->setLedsToError();
            agent->reconect();
        }
        sensor->setLedsToNormal();
        this->setState(COMPUTING);
        break;
    case COMPUTING:
        this->logOnce("[OBSERVER] : COMPUTING");
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