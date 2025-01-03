#include "SmartTemperatureSensor.h"


#define GREEN_LED 1
#define RED_LED 2
#define TEMPERATURE_SENSOR 3

SmartTemperatureSensor::SmartTemperatureSensor()
{
    greenLed = new Led(GREEN_LED);
    redLed = new Led(RED_LED);
    sensor = new TempSensorLM35(TEMPERATURE_SENSOR);
}

void SmartTemperatureSensor::setLedsToNormal()
{
    this->greenLed->switchOn();
    this->redLed->switchOff();
}

void SmartTemperatureSensor::setLedsToError()
{
    this->greenLed->switchOff();
    this->redLed->switchOn();
}

float SmartTemperatureSensor::getTemperature()
{
    return this->sensor->getTemperature();
}

void SmartTemperatureSensor::setFrequency(int frequency){
    
}
