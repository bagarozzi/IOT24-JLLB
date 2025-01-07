#include<WiFi.h>
#include <WiFiClient.h>
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
    Serial.println("arrivato json");
    String app;
    for( int i = 0; i< sizeof(payload); i++)
    {
        app[i] = payload[i];
    }
    buffer.push_back(app);
}


MQTT_agent::MQTT_agent(const char* mqtt_server, int port, const char* topic, const char* ssid, const char* password) :
    ssid(ssid), password(password), mqtt_server(mqtt_server), topic(topic)
{
    this->setUpWiFi();
    client.setServer(mqtt_server, port);
    client.subscribe(topic);
    client.setCallback(callback);
}

void MQTT_agent::setUpWiFi(){
    Serial.println(String("tring to connect to: ") + "Luca");
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
            client.subscribe(topic);
        } else {
            Serial.println("not connected");
            Serial.println(client.state() + "\n");
        }
    }
}

String MQTT_agent::reciveMessage() {
    Serial.print("recieve");
    String app = buffer.front();
    buffer.pop_front();
    return app;
}

void MQTT_agent::sendMessage(String message){
    client.publish(topic, message.c_str());
}

bool MQTT_agent::isMessageArrived()
{
    return !buffer.empty();
}
