#include <Task.h>
#include "MQTT_Agent.h"
#include "TempSensorLM35.h"
#include "SmartTemperatureSensor.h"
class TemperatureTask : public Task
{
private:
    SmartTemperatureSensor temperatureSensor;
    MQTT_agent agent;

    enum State{
            NORMAL,
            FAST,
            ALARM,
        };
public:
    TemperatureTask(MQTT_agent agent, SmartTemperatureSensor sensor);
    void tick();
};
