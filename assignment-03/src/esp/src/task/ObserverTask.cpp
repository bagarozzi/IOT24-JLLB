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
            //recieve the message and convert into a Json
            String msg = task->agent->reciveMessage();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, msg);
            int freq = (int) doc["frequency"];
            //send the frequency into the queue           
            task->frequencyQueue->send(freq);
            task->setState(IDLE);
        }
        break;
        case RECONNECTING:
        {
            if(task->isJustEntered())
            {
                task->sensor->setLedsToError();
            }
            task->logOnce("[OBSERVER] : RECONNECTING");
            task->agent->reconect();
            if(task->agent->isConnected())
            {
                task->setState(IDLE);
                task->sensor->setLedsToNormal();
            }
        }
        break;
        }
        vTaskDelay(100);
    }
}