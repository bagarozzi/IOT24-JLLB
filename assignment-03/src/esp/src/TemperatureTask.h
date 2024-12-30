#include <Task.h>
#include "MQTT_Agent.h"
class TemperatureTask : public Task
{
private:
    MQTT_agent agent;
    //TempSensor sensor;
    enum State{
        
            NORMAL,
            FAST,
            ALARM,
        };
public:
    TemperatureTask();
    void tick();
};
