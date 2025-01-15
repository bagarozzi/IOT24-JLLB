package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.mqtt.MqttClient;

import it.unibo.smartmonitoring.Configuration;

/**
 * An MQTT client that connects to the {@link MQTTServer} and
 * subscribes to the topic where the ESP32 publishes the data.
 * <br>
 * With every new MQTT message, the client publishes it on the 
 * event bus.
 */
public class TestMQTTMessages extends AbstractVerticle {
	
	public TestMQTTMessages() { }

	@Override
	public void start() {		
		MqttClient client = MqttClient.create(vertx);

		log("connecting to \"" + Configuration.MQTT_BROKER_ADDRESS + "\"");
		
		client.connect(Configuration.MQTT_BROKER_PORT, Configuration.MQTT_BROKER_ADDRESS, c -> {

			if(c.succeeded()) {
				log("Connected to \"" + Configuration.MQTT_BROKER_ADDRESS + "\"");
				log("Subscribing to: \"" + Configuration.ESP_FREQUENCY_TOPIC + "\"");
				client.publishHandler(message -> {
					System.out.println("[MQTT-AGENT]: Received message from ESP32");
					System.out.println(message.payload().toString());
				})
				.subscribe(Configuration.ESP_FREQUENCY_TOPIC, 0, s -> {
					if(s.succeeded()) {
						log("Subscribed to: \"" + Configuration.ESP_FREQUENCY_TOPIC + "\"");
					}
					else if(s.failed()) {
						log("Failed to subscribe to: \"" + Configuration.ESP_FREQUENCY_TOPIC + "\"");
					}
				});
			}
			else if(c.failed()) {
				log("Failed to connect to \"" + Configuration.MQTT_BROKER_ADDRESS + "\"");
				return;
			}
		});
	}
	

	private void log(String msg) { 
		System.out.println("[MQTT-AGENT]: " + msg);
	}



}