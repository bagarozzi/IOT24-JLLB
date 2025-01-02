#include <List>
class MQTT_agent {

    public :
        MQTT_agent();
        boolean isConnected();
        void reconect();
        String reciveMessage();
        void sendMessage(String message);
        bool isMessageArrived();
        
    
    private :
        const char* ssid = "LittleBarfly";
        const char* password = "esiot-2024-2025";
        const char* mqtt_server = "broker.mqtt-dashboard.com";
        const char* topic = "esiot-2024";
        std::list<String> buffer;

        void addMessage(String message);
        void setUpWiFi();
        
};