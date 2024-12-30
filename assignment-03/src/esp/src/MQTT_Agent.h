
class MQTT_agent {

    public :
        MQTT_agent();
        boolean isConnected();
        void reconect();
        String reciveMessage();
        void sendMessage(String message);
    
    private :
        const char* ssid = "LittleBarfly";
        const char* password = "esiot-2024-2025";
        const char* mqtt_server = "broker.mqtt-dashboard.com";
        const char* topic = "esiot-2024";
        
        void setUpWiFi();
        
};