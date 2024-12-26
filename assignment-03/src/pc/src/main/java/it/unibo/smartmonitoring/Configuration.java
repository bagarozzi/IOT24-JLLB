package it.unibo.smartmonitoring;

public final class Configuration {
    
    /* MQTT Broker and client setup */
    public static final String MQTT_BROKER_ADDRESS = "broker.mqtt-dashboard.com";
	public static final String ESP_TOPIC_NAME = "temperature/smartwindow";

    /* Event bus addresses: */
    public static final String BACKEND_MQTT_EB_ADDR = "mqtt.backend.com";
    public static final String BACKEND_HTTP_EB_ADDR = "http.backend.com";
    public static final String BACKEND_ARDUINO_EB_ADDR = "arduino.backend.com";

}
