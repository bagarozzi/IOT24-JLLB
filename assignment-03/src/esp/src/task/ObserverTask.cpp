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
    case IDLE: {
        if (this->isJustEntered() && agent->isConnected())
        {
            sensor->setLedsToNormal();
        }
        else if (!agent->isConnected())
        {
            this->setState(RECONNECTING);
        }
        this->logOnce("[OBSERVER] : IDLE");

        if (agent->isMessageArrived())
        {
            this->setState(COMPUTING);
        }
    }
    break;
    case COMPUTING: {
        this->logOnce("[OBSERVER] : COMPUTING");
        String msg = agent->reciveMessage();
        JsonDocument doc;
        deserializeJson(doc, msg);
        sensor->setFrequency(doc["frequency"]);
        this->setState(IDLE);
    }
    break;
    case RECONNECTING: {
        this->logOnce("[OBS] : RECONNECTING");
        sensor->setLedsToError();
        while (!agent->isConnected())
        {
            Serial.println("connecting");
            sensor->setLedsToError();
            agent->reconect();
        }
        this->setState(IDLE);
    }
    break;
    }
}