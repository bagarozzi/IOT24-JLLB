
#include "TemperatureTask.h"

#define BASE_FREQUENCY 500

TemperatureTask::TemperatureTask(MQTT_agent *agent, SmartTemperatureSensor *sensor, TaskQueue<int> *queue) : frequencyQueue(queue), agent(agent), sensor(sensor)
{
    frequency = BASE_FREQUENCY;
    this->setState(IDLE);
}

void TemperatureTask::tick(void *parameter)
{
    TemperatureTask *task = static_cast<TemperatureTask *>(parameter);

    while (true)
    {
        switch (task->getState())
        {
        case IDLE:
            task->logOnce("[TEMP] : IDLE ");
            if(!task->agent->isConnected())
            {
                task->setState(CONNECTING);
            }
            task->getFrequency();
            if (task->elapsedTimeInState() >= task->frequency)
            {
                task->setState(SENDIG);
            }
            break;
        case SENDIG:
            task->logOnce("[TEMP] : SENDING : " + (String)task->sensor->getTemperature());
            task->agent->sendMessage((String)task->sensor->getTemperature());
            task->setState(IDLE);
            break;
        case CONNECTING:
            task->logOnce("[TEMP] : RECONNECTING");
            if(task->agent->isConnected())
            {
                task->setState(IDLE);
            }
        }
        vTaskDelay(100);
    }
}

void TemperatureTask::getFrequency()
{
    int freq = frequencyQueue->recieve();
    if(freq != pdFALSE)
    {
        frequency = freq;
    }
}