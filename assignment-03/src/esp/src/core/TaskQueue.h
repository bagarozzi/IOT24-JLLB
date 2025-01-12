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
        //initialize the queue
        queue = xQueueCreate(queueSize, itemSize);
    }

    //return the item if it present
    T recieve()
    {   
        T item;
        if(xQueueReceive(queue, &item, 0) == pdFALSE)
        {
            return pdFALSE;
        }
        return item;
    }

    //send an item in the queue
    void send(T item)
    {
        xQueueSend(queue, &item, 0);
    }
}; 

#endif

