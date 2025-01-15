package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;

public final class TestVerticle extends AbstractVerticle {

    private final int testChioce = 0;

    public void start() {
        switch (testChioce) {
            case 0:
                testNormal();
                break;
            case 1:
                testManual();
                break;
            default:
                break;
        }
    }

    private void testManual() {
        vertx.setTimer(4000, id -> {
            vertx.eventBus().send(
            Configuration.BACKEND_MQTT_EB_ADDR,
            new JsonObject()
                .put("temperature", 20)
            );
        });
        vertx.setTimer(5000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode", "manual")
            );
        });
        vertx.setTimer(7000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-aperture")
                    .put("aperture", 50)
            );
        });
        vertx.setTimer(9000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode","auto")
            );
        });
        vertx.setTimer(10000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR,
                new JsonObject()
                    .put("temperature", 28)
            );
        });
        vertx.setTimer(12000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode","manual")
            );
        });
        vertx.setTimer(14000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-aperture")
                    .put("aperture", 60)
            );
        });
        vertx.setTimer(16000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode","auto")
            );
        });
        vertx.setTimer(18000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_MQTT_EB_ADDR,
                new JsonObject()
                    .put("temperature", 36)
            );
        });
        vertx.setTimer(20000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode","manual")
            );
        });
        vertx.setTimer(22000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-aperture")
                    .put("aperture", 60)
            );
        });
        vertx.setTimer(24000, id -> {
            vertx.eventBus().send(
                Configuration.BACKEND_HTTP_EB_ADDR,
                new JsonObject()
                    .put("type", "set-mode")
                    .put("mode","auto")
            );
        });
    }

    private void testNormal() {
        System.out.println("Standard est started");
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
