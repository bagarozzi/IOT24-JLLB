package it.unibo.smartmonitoring;

public final class Configuration {

    /* MQTT Broker and client setup */
    public static final String MQTT_BROKER_ADDRESS = "broker.hivemq.com";
    public static final int MQTT_BROKER_PORT = 1883;
    public static final String ESP_TEMPERATURE_TOPIC = "it/unibo/smartmonitoring/temperature/smartwindow";
    public static final String ESP_FREQUENCY_TOPIC = "it/unibo/smartmonitoring/frequency/smartwindow";

    /* Event bus addresses: */
    public static final String BACKEND_MQTT_EB_ADDR = "mqtt.backend.com";
    public static final String BACKEND_HTTP_EB_ADDR = "http.backend.com";
    public static final String BACKEND_ARDUINO_EB_ADDR = "arduino.backend.com";

    public static final String ARUDINO_EB_ADDR = "window.arduino.com";

    public static final String HTTP_EB_ADDR = "dashboard.http.com";

    public static final String MQTT_EB_ADDR = "mqtt.thermometer.com";

    /* Backend variables for automatic mode: */
    public static final float NORMAL_MODE_THRESHOLD = 27.0f;
    public static final float HOT_MODE_THRESHOLD = 35.0f;
    public static final long ALARM_THRESHOLD_TIME = 10000;
    public static final int NORMAL_MODE_POLLING_FREQUENCY = 500;
    public static final int HOT_MODE_POLLING_FREQUENCY = 200;
    public static final int TEMPERATURE_LOG_SIZE = 3000;
}
