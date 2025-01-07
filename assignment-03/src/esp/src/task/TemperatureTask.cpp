
#include "TemperatureTask.h"

#define BASE_FREQUENCY 5000

TemperatureTask::TemperatureTask(MQTT_agent *agent, SmartTemperatureSensor *sensor, TaskQueue<int> *queue) : frequencyQueue(queue), agent(agent), sensor(sensor)
{
    frequency = BASE_FREQUENCY;
    this->setState(IDLE);
}

void TemperatureTask::tick(void *parameter)
{
    Serial.println("inizio");
    TemperatureTask *task = static_cast<TemperatureTask *>(parameter);

    while (true)
    {
        switch (task->getState())
        {
        case IDLE:
            task->logOnce("[TEMP] : IDLE ");
            task->getFrequency();
            if (task->elapsedTimeInState() >= task->frequency)
            {
                task->setState(SENDIG);
            }
            break;
        case SENDIG:
            task->logOnce("[TEMP] : SENDING : " + (String)task->sensor->getTemperature());
            task->agent->sendMessage((String)task->sensor->getTemperature());
            Serial.println("ciao");
            task->setState(IDLE);
            break;
        }
        vTaskDelay(100);
    }
}

void TemperatureTask::getFrequency()
{
    int freq = frequencyQueue->recieve();
    if (freq == pdTRUE)
    {
        Serial.println("frequency arrived");
        frequency = freq;
    }
}