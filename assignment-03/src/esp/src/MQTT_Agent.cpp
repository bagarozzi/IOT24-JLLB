#include<WiFi.h>
#include<PubSubClient.h>
#include<MQTT_Agent.h>

WiFiClient espClient;
PubSubClient client(espClient);



void MQTT_agent::setUpWiFi(){
    Serial.println(String("tring to connect to: ") + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("\nconnected\n");
}

void callback(char* topic, uint8_t * payload, unsigned int length) {
    
}

MQTT_agent::MQTT_agent(){
    setUpWiFi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

boolean MQTT_agent::isConnected(){
    return client.connected();
}

void MQTT_agent::reconect(){
    while(!client.connected()){
        if(client.connect("SmartTemperatureSensor")) {
            Serial.println("isConnected\n");
        } else {
            Serial.println(client.state()+"\n");
        }
    }
}

String MQTT_agent::reciveMessage() {
    String app = this->buffer.front();
    buffer.pop_front();
    return app;
}

void MQTT_agent::sendMessage(String message){
    client.publish("temperature", message.c_str());
}

bool MQTT_agent::isMessageArrived()
{
    return !buffer.empty();
}

void MQTT_agent::addMessage(String message){
    this->buffer.push_back(message);
}
