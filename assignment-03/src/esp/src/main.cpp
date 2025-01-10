#include <Arduino.h>
#include "model/SmartTemperatureSensor.h"
#include "task/TemperatureTask.h"
#include "task/ObserverTask.h"
#include <Wifi.h>
#include "core/TaskQueue.h"

// put function declarations here:
int myFunction(int, int);

TaskHandle_t Task1;
TaskHandle_t Task2;

ObserverTask* observerTask;
TemperatureTask* temperatureTask;

MQTT_agent* agent;

void setup()
{
	TaskQueue<int>* queue = new TaskQueue<int> (sizeof(int), 10);
	Serial.begin(115200);

	//initialize the temperatureSensor and mqttAgent
	SmartTemperatureSensor *sensor = new SmartTemperatureSensor();
	agent = new MQTT_agent("broker.hivemq.com", 1883, "it/unibo/smartmonitoring/temperature/smartwindow",
		"it/unibo/smartmonitoring/frequency/smartwindow", "Luca", "luca1234");

	Serial.println("superato");
	observerTask = new ObserverTask(sensor, agent, queue);
	observerTask->init(100);

	temperatureTask = new TemperatureTask(agent, sensor, queue);
	temperatureTask->init(100);

	xTaskCreatePinnedToCore(TemperatureTask::tick, "Task1", 10000, temperatureTask, 1, &Task1,0);                         

  	xTaskCreatePinnedToCore(ObserverTask::tick, "Task2", 10000, observerTask, 1, &Task2,1);

}

void loop()
{
	agent->loop();
}