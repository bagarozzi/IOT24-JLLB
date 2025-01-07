#ifndef __TASK_QUEUE__
#define __TASK_QUEUE__

#include <Arduino.h>

template<typename T>

class TaskQueue
{
private:

    xQueueHandle queue;

public:

    TaskQueue(int itemSize, int queueSize)
    {
        queue = xQueueCreate(queueSize, itemSize);
    }

    T recieve()
    {   
        T item;
        xQueueReceive(queue, &item, 0);
        if(item == pdTRUE)
        {
            Serial.println("frequency" + item);
        }
        return (T)item;
    }

    void send(T item)
    {
        xQueueSend(queue, &item, 0);
    }
}; 

#endif

