#include <Arduino.h>
#include "core/Scheduler.h"
#include "model/SmartTemperatureSensor.h"
#include "task/TemperatureTask.h"
#include "task/ObserverTask.h"
#include "core/Scheduler.h"
#
// put function declarations here:
int myFunction(int, int);

void setup()
{

	// initialize the temperatureSensor, scheduler and mqttAgent
	SmartTemperatureSensor *sensor = new SmartTemperatureSensor();

	Scheduler *scheduler = new Scheduler();
	scheduler->init(100);

	MQTT_agent *agent = new MQTT_agent("", "", "", "");

	// initialize and add task to the scheduler
	scheduler->addTask(new ObserverTask(sensor, agent));
	scheduler->addTask(new TemperatureTask(agent, sensor));
}

void loop()
{
}

// put function definitions here:
int myFunction(int x, int y)
{
	return x + y;
}