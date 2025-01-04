package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;
import it.unibo.smartmonitoring.core.api.BackendVerticle;

public class MainVerticle extends AbstractVerticle {

	@Override
	public void start(Promise<Void> startPromise) throws Exception {
		BackendVerticle backendVerticle = new BackendVerticleImpl();
		MQTTClientVerticle mqttClientVerticle = new MQTTClientVerticle();
		vertx.deployVerticle(mqttClientVerticle);
		vertx.deployVerticle(backendVerticle);
		//TestVerticle testVerticle = new TestVerticle();
		//vertx.deployVerticle(backendVerticle, ar -> vertx.deployVerticle(testVerticle));
    	vertx.deployVerticle(new HTTPVerticle());
	}
}
