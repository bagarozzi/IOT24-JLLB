#include "ObserverTask.h"
#include <ArduinoJson.h>

void ObserverTask::tick()
{
    if(agent.isMessageArrived())
    {
        String msg = agent.reciveMessage();
        StaticJsonDocument<100> doc;
        deserializeJson(doc, msg);

        if(doc["state"] == "hot")
        {
            sensor.setState(1);
        } else if (doc["state"] == "too-hot")
        {
            sensor.setState(2);
        } else if (doc["state"] == "alarm")
        {
            sensor.setState(3);
        }
        
    }
}