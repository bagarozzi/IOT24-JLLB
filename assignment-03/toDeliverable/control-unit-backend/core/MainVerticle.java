/*
 * ASSIGNMENT #3
 *
 * Author:
 * Bagattoni Federico
 * Ponseggi Luca
 * Turchi Jacopo
 * Venturini Luca
 *
 */
package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.serial.impl.SerialCommChannel;


public class MainVerticle extends AbstractVerticle {

	@Override
	public void start(Promise<Void> startPromise) throws Exception {
		SerialCommChannel channel = new SerialCommChannel("/dev/cu.usbmodem1301", 115200);
		BackendVerticle backendVerticle = new BackendVerticleImpl();
		MQTTClientVerticle mqttClientVerticle = new MQTTClientVerticle();
		SerialVerticle serialVerticle = new SerialVerticle(channel);
		vertx.deployVerticle(mqttClientVerticle);
		vertx.deployVerticle(backendVerticle);
		vertx.deployVerticle(serialVerticle);
		//TestVerticle testVerticle = new TestVerticle();
		//vertx.deployVerticle(backendVerticle, ar -> vertx.deployVerticle(testVerticle));
    	vertx.deployVerticle(new HTTPVerticle());
	}
}
