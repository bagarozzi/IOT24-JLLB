#ifndef __MQTT_AGENT__
#define __MQTT_AGENT__

#include <List>
#include <Arduino.h>

class MQTT_agent {

    public :
        MQTT_agent(const char* ssid, const char* password, const char* mqtt_server, const char* topic);
        bool isConnected();
        void reconect();
        String reciveMessage();
        void sendMessage(String message);
        bool isMessageArrived();
        
    
    private :
        const char* ssid; //= "LittleBarfly";
        const char* password; //= "esiot-2024-2025";
        const char* mqtt_server; //= "broker.mqtt-dashboard.com";
        const char* topic; //= "esiot-2024";
        std::list<String> buffer;

        void addMessage(String message);
        void setUpWiFi();
        
};

#endif