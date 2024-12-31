package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;

public final class TestVerticle extends AbstractVerticle {
    
    public void start() {
        System.out.println("Test started");
        vertx.eventBus().send(
            Configuration.BACKEND_MQTT_EB_ADDR, 
            new JsonObject()
                .put("temperature", 20)
        );
        vertx.setTimer(5000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 25)
            );
        });
        vertx.setTimer(10000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 26)
            );
        });
        vertx.setTimer(12000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 27)
            );
        });
        vertx.setTimer(13000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 28)
            );
        });
        vertx.setTimer(17000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 36)
            );
        });
        vertx.setTimer(26000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 25)
            );
        });
        vertx.setTimer(29000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 36)
            );
        });
        vertx.setTimer(41000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR, 
                new JsonObject()
                    .put("temperature", 20)
            );
        });
        vertx.setTimer(50000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR, 
                new JsonObject()
                    .put("type", "reset-alarm")
            );
        });
    }
}
