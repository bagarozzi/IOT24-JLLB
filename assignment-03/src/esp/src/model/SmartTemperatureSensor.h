#ifndef __SMART_TEMPERATURE_SENSOR__
#define __SMART_TEMPERATURE_SENSOR__

#include "Led.h"
#include "TempSensor.h"
#include "TempSensorLM35.h"
#include <Arduino.h>

class SmartTemperatureSensor
{
    private:
        Led* greenLed;
        Led* redLed;
        TempSensor* sensor;

    public:
        SmartTemperatureSensor();
        void setLedsToNormal();
        void setLedsToError();
        float getTemperature();
        void setFrequency(int frequency);
};


#endif