#include<WiFi.h>
#include <WiFiClient.h>
#include<ArduinoJson.h>
#include<PubSubClient.h>
#include "MQTT_Agent.h"

WiFiClient espClient;
PubSubClient client(espClient);

std::list<String> buffer;
        

String toString(uint8_t* data)
{
    String* app;
    for( int i = 0; i< sizeof(data); i++)
    {
        app[i] = data[i];
    }
    return *app;
}

void callback(char* topic, uint8_t * payload, unsigned int length) {
    Serial.println("arrivato");
    String app;
    for(int i = 0; i < length; i++)
    {
        app += (char)payload[i];
    }

    buffer.push_back(app);
}


MQTT_agent::MQTT_agent(const char* mqtt_server, int port, const char* ssid, const char* password) :
    mqtt_server(mqtt_server), port(port), ssid(ssid), password(password)
{
    publishTopic = "it/unibo/smartmonitoring/temperature/smartwindow";
    recieveTopic = "it/unibo/smartmonitoring/frequency/smartwindow";
    this->setUpWiFi();
    client.setServer(mqtt_server, port);
    client.subscribe(recieveTopic);
    client.setCallback(callback);
}

void MQTT_agent::setUpWiFi(){
    Serial.println(String("tring to connect to: ") + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("\nwifi connected\n");
}

boolean MQTT_agent::isConnected(){
    return client.connected();
}

void MQTT_agent::reconect(){
    while(!client.connected()){
        if(client.connect("SmartTemperatureSensor")) {
            Serial.println("isConnected\n");
            client.subscribe(recieveTopic);
            client.setCallback(callback);
        } else {
            Serial.println("not connected");
            Serial.println(client.state() + "\n");
        }
    }
}

String MQTT_agent::reciveMessage() {
    String app = buffer.front();
    buffer.pop_front();
    return app;
}

void MQTT_agent::sendMessage(String message){
    client.publish(publishTopic, message.c_str());
}

bool MQTT_agent::isMessageArrived()
{
    return !buffer.empty();
}

void MQTT_agent::loop()
{
    client.loop();
}
