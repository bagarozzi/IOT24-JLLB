package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.mqtt.MqttServer;

public class MQTTServer extends AbstractVerticle {

	@Override
	public void start() {
		Vertx vertx = Vertx.vertx();
		MqttServer mqttServer = MqttServer.create(vertx);
		mqttServer.endpointHandler(endpoint -> {
	
		  // shows main connect info
		  log("MQTT client [" + endpoint.clientIdentifier() + "] request to connect, clean session = " + endpoint.isCleanSession());
		
		  if (endpoint.auth() != null) {
			log("username = " + endpoint.auth().getUsername() + ", password = " + endpoint.auth().getPassword());
		  }
		  if (endpoint.will() != null) {
			log("will topic = " + endpoint.will().getWillTopic() + " msg = " + new String(endpoint.will().getWillMessageBytes()) +
			  " QoS = " + endpoint.will().getWillQos() + " isRetain = " + endpoint.will().isWillRetain());
		  }
	
		  System.out.println("keep alive timeout = " + endpoint.keepAliveTimeSeconds());
	
		  // accept connection from the remote client
		  endpoint.accept(false);
	
		})
		  .listen(ar -> {
			if (ar.succeeded()) {
			  log("Listening on port " + ar.result().actualPort());
			} else {
	
			  log("Error on starting the server");
			  ar.cause().printStackTrace();
			}
		  });
	}

	private void log(final String msg) {
		System.out.println("[MQTT-SERVER]: " + msg);
	}
}