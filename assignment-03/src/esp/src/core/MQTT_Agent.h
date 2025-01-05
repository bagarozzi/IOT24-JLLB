#ifndef __MQTT_AGENT__
#define __MQTT_AGENT__

#include <List>
#include <Arduino.h>

class MQTT_agent {

    public :
        MQTT_agent(const char* mqtt_server, int port, const char* topic, const char* ssid, const char* password);
        bool isConnected();
        void reconect();
        String reciveMessage();
        void sendMessage(String message);
        bool isMessageArrived();
        
    
    private :
        const char* ssid;
        const char* password;
        const char* mqtt_server;
        const char* topic;
        const char* mqtt_username = "SmartTemperatureSensor";
        const char* mqtt_password = "Luca1234";
        void setUpWiFi();
};

#endif