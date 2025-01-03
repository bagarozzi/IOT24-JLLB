package it.unibo.smartmonitoring.core;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.eventbus.EventBus;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonObject;
import io.vertx.mqtt.MqttClient;

import it.unibo.smartmonitoring.Configuration;

/**
 * An MQTT client that connects to the {@link MQTTServer} and
 * subscribes to the topic where the ESP32 publishes the data.
 * <br>
 * With every new MQTT message, the client publishes it on the 
 * event bus.
 */
public class MQTTClientVerticle extends AbstractVerticle {
	
	public MQTTClientVerticle() { }

	@Override
	public void start() {		
		MqttClient client = MqttClient.create(vertx);
        EventBus eb = vertx.eventBus();
		
		client.connect(Configuration.MQTT_BROKER_PORT, Configuration.MQTT_BROKER_ADDRESS, c -> {

			log("Connected to \"" + Configuration.MQTT_BROKER_ADDRESS + "\"");
			log("Subscribing to: \"" + Configuration.ESP_TOPIC_NAME + "\"");
			client.publishHandler(message -> {
                System.out.println("[MQTT-AGENT]: Received message from ESP32");
                eb.send(Configuration.BACKEND_MQTT_EB_ADDR, toJson(message.payload().toString()));
			})
			.subscribe(Configuration.ESP_TOPIC_NAME, 2);		
		});

		eb.consumer(Configuration.MQTT_EB_ADDR, message -> {
			client.publish("temperature", null, MqttQoS.AT_LEAST_ONCE, false, false);
		});
	}
	

	private void log(String msg) { 
		System.out.println("[MQTT-AGENT]: " + msg);
	}

	private JsonObject toJson(String temperature) {
		return new JsonObject().put("temperature", Float.valueOf(temperature));
	}



}