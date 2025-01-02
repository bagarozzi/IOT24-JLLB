#include <Task.h>
#include "MQTT_Agent.h"
#include "SmartTemperatureSensor.h"
class ObserverTask : public Task {
    private:
        enum {
            LISTENING,
            COMPUTING,
        };
        
        SmartTemperatureSensor sensor;
        MQTT_agent agent;

    public:
        void tick();
};
