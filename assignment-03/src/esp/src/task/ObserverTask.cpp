#include "ObserverTask.h"
#include <ArduinoJson.h>

ObserverTask::ObserverTask(SmartTemperatureSensor *sensor, MQTT_agent *agent, TaskQueue<int> *queue) : frequencyQueue(queue), sensor(sensor), agent(agent)
{
    this->setState(IDLE);
}

void ObserverTask::tick(void *parameter)
{
    ObserverTask *task = static_cast<ObserverTask *>(parameter);
    if(task->agent->isConnected())
    {
        task->sensor->setLedsToNormal();
    }
    while (true)
    {
        switch (task->getState())
        {
        case IDLE:
        {
            task->logOnce("[OBSERVER] : IDLE");
            if (!task->agent->isConnected())
            {
                task->setState(RECONNECTING);
            }
            if (task->agent->isMessageArrived())
            {
                task->setState(COMPUTING);
            }
        }
        break;
        case COMPUTING:
        {
            task->logOnce("[OBSERVER] : COMPUTING");
            String msg = task->agent->reciveMessage();
            JsonDocument doc;
            deserializeJson(doc, msg);
            task->frequencyQueue->send(doc["frequency"]);
            task->setState(IDLE);
        }
        break;
        case RECONNECTING:
        {
            task->logOnce("[OBS] : RECONNECTING");
            task->sensor->setLedsToError();
            while (!task->agent->isConnected())
            {
                task->sensor->setLedsToError();
                task->agent->reconect();
            }
            task->setState(IDLE);
        }
        break;
        }
        vTaskDelay(100);
    }
}