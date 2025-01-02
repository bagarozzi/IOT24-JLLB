#include "Task.h"
#include "Led.h"
#include "TempSensor.h"
#include "TempSensorLM35.h"
class SmartTemperatureSensor : public Task
{
    private:
        enum State{
            NORMAL,
            HOT,
            TOO_HOT,
            ALARM,
        };

        int state;
        Led* greenLed;
        Led* redLed;
        TempSensor* sensor;

    public:
        SmartTemperatureSensor();


        void setStateAlarm();
        void setState(int state);
        void setLedsToNormal();
        void setLedsToError();
        float getTemperature();
        bool isInAlarmState();
        bool isInHotState();
        
};

