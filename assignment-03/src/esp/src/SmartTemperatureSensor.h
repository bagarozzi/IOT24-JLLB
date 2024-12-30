#include "Task.h"
class SmartTemperatureSensor : public Task
{
    private:
        enum State{
        
            NORMAL,
            FAST,
            ALARM,
        };

    public:
        SmartTemperatureSensor(/* args */);
};

