package it.unibo.smartmonitoring;

import io.vertx.core.Vertx;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonObject;
import io.vertx.junit5.VertxExtension;
import io.vertx.junit5.VertxTestContext;
import it.unibo.smartmonitoring.core.BackendVerticleImpl;
import it.unibo.smartmonitoring.core.api.BackendVerticle;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;

@ExtendWith(VertxExtension.class)
public class TestBackendVerticle {

	private BackendVerticle backendVerticle; 
	
	@BeforeEach
	void deploy_verticle(Vertx vertx, VertxTestContext testContext) {
		/*backendVerticle = new BackendVerticleImpl();
		vertx.deployVerticle(backendVerticle,
		ar -> {
			if(ar.succeeded()) {
				testContext.completeNow();
			} else {
				testContext.failNow(ar.cause());
			}
		});*/
	}

	@Test
	void testNormalState(Vertx vertx, VertxTestContext testContext) {
		backendVerticle = new BackendVerticleImpl();
		vertx.deployVerticle(backendVerticle, ar -> {
			if (ar.succeeded()) {
				sendTemperature(vertx, 20.0f);
				vertx.setTimer(500, id -> {
					if (backendVerticle.getState().equals(BackendVerticle.State.NORMAL)) {
						testContext.completeNow();
					} else {
						testContext.failNow(new Throwable("State is not NORMAL. Is " + backendVerticle.getState()));
					}
				});
			} else {
				testContext.failNow(ar.cause());
			}
		});
	}

	@Test
	void testHotState(Vertx vertx, VertxTestContext testContext) {
		backendVerticle = new BackendVerticleImpl();
		vertx.deployVerticle(backendVerticle, ar -> {
			if (ar.succeeded()) {
				sendTemperature(vertx, 28.0f);
				vertx.setTimer(500, id -> {
					if (backendVerticle.getState().equals(BackendVerticle.State.HOT)) {
						testContext.completeNow();
					} else {
						testContext.failNow(new Throwable("State is not HOT. Is " + backendVerticle.getState()));
					}
				});
			} else {
				testContext.failNow(ar.cause());
			}
		});
	}

	void sendTemperature(Vertx vertx, final float temp) {
		vertx.eventBus().send(
			Configuration.BACKEND_MQTT_EB_ADDR,
			new JsonObject()
				.put("temperature", temp)
		);
	}
	
}
