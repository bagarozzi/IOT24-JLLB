#include <Arduino.h>
#include "core/Scheduler.h"
#include "model/SmartTemperatureSensor.h"
#include "task/TemperatureTask.h"
#include "task/ObserverTask.h"
#include "core/Scheduler.h"
#include <Wifi.h>

// put function declarations here:
int myFunction(int, int);

Scheduler *scheduler;

void setup()
{
	Serial.begin(115200);

	// initialize the temperatureSensor, scheduler and mqttAgent
	SmartTemperatureSensor *sensor = new SmartTemperatureSensor();
	
	scheduler = new Scheduler();
	scheduler->init(100);
	Serial.println("agent");
	MQTT_agent *agent = new MQTT_agent("02d38b12a6fe4ce9aecd2a5679225e84.s1.eu.hivemq.cloud", 8883, "tempertature", "Luca", "luca1234");

	ObserverTask* observerTask = new ObserverTask(sensor, agent);
	observerTask->init(100);

	TemperatureTask* temperatureTask = new TemperatureTask(agent, sensor);
	temperatureTask->init(100);

	// initialize and add task to the scheduler
	scheduler->addTask(observerTask);
	scheduler->addTask(temperatureTask);
}

void loop()
{
	scheduler->schedule();
}
