package it.unibo.smartmonitoring;

public final class Configuration {
    
    /* MQTT Broker and client setup */
    public static final String MQTT_BROKER_ADDRESS = "broker.mqtt-dashboard.com";
	public static final String ESP_TOPIC_NAME = "temperature/smartwindow";

    /* Event bus addresses: */
    public static final String BACKEND_MQTT_EB_ADDR = "mqtt.backend.com";
    public static final String BACKEND_HTTP_EB_ADDR = "http.backend.com";
    public static final String BACKEND_ARDUINO_EB_ADDR = "arduino.backend.com";

    public static final String ARUDINO_EB_ADDR = "window.arduino.com";

    public static final String MQTT_EB_ADDR = "mqtt.thermometer.com";

    /* Backend variables for automatic mode: */
    public static final float NORMAL_MODE_THRESHOLD = 27.0f;
    public static final float HOT_MODE_THRESHOLD = 35.0f;
    public static final long ALARM_THRESHOLD_TIME = 10000;
    public static final int NORMAL_MODE_POLLING_FREQUENCY = 500;
    public static final int HOT_MODE_POLLING_FREQUENCY = 200;
}
