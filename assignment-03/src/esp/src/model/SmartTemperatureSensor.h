#ifndef __SMART_TEMPERATURE_SENSOR__
#define __SMART_TEMPERATURE_SENSOR__

#include "Led.h"
#include "TempSensor.h"
#include "TempSensorLM35.h"
#include <Arduino.h>

class SmartTemperatureSensor
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

        int getState();
        void setStateAlarm();
        void setState(int state);
        void setLedsToNormal();
        void setLedsToError();
        float getTemperature();
        bool isInAlarmState();
        bool isInHotOrTooHotState();
        
};


#endif