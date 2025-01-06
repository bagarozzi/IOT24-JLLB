#include <Arduino.h>

template<typename T>

class TaskQueue
{
private:

    xQueueHandle queue;

public:

    TaskQueue(int itemSize, int queueSize)
    {
        queue = xQueueCreate(itemSize, queueSize);
    }

    T recieve()
    {   
        T item;
        xQueueReceive(queue, item, 0);
    }

    void send(T item)
    {
        xQueueSend(queue, item, 0);
    }
};

