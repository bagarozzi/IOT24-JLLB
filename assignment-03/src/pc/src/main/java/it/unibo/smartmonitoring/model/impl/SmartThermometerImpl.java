package it.unibo.smartmonitoring.model.impl;

import java.util.ArrayDeque;
import java.util.Collections;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.utils.MessageParser;

public class SmartThermometerImpl extends AbstractVerticle implements SmartThermometer {

    private final ArrayDeque<Float> temperatureLogs;
    private int frequency;

    private final BackendVerticle backend;

    public SmartThermometerImpl(final BackendVerticle backendVerticle) {
        this.backend = backendVerticle;
        temperatureLogs = new ArrayDeque<>();  
        temperatureLogs.add(0.0f);
    }

    @Override
    public void start() {
        setEventBusConsumer();
        System.out.println("[BACKEND]: SmartThermometer deployment completed");
    }

    @Override
    public void setFrequency(int frequency) {
        this.frequency = frequency;
        sendFrequency();
    }

    @Override
    public float getTemperature() {
        return temperatureLogs.getLast();
    }

    @Override
    public void setTemperature(float temperature) {
        temperatureLogs.removeFirst();
        temperatureLogs.add(temperature);
    }

    @Override
    public float getMinTemperature() {
        return Collections.min(temperatureLogs);
    }

    @Override
    public float getMaxTemperature() {
        return Collections.max(temperatureLogs);
    }

    @Override
    public float getAverageTemperature() {
        return (float) temperatureLogs.stream()
            .mapToDouble(Float::doubleValue)
            .average()
            .orElse(0.0);
    }

    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_MQTT_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from MQTT verticle temp " + (JsonObject)message.body()); 
            setTemperature(MessageParser.parseMQTTMessage((JsonObject)message.body()));
        });
    }

    private void sendFrequency() {
        vertx.eventBus().send(
            Configuration.MQTT_EB_ADDR,
            new JsonObject()
                .put("frequency", this.frequency)
        );
    }

}
