package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.utils.MessageParser;

public class SmartThermometerImpl extends AbstractVerticle implements SmartThermometer {

    private float temperature;
    private int frequency;

    @Override
    public void start() {
        setEventBusConsumer();
    }

    @Override
    public void setFrequency(int frequency) {
        this.frequency = frequency;
        sendFrequency();
    }

    @Override
    public float getTemperature() {
        return this.temperature;
    }

    @Override
    public void setTemperature(float temperature) {
        this.temperature = temperature;
    }

    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_MQTT_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from MQTT verticle");
            setTemperature(MessageParser.parseMQTTMessage((JsonObject)message.body()));
        });
    }

    private void sendFrequency() {
        //vertx.eventBus().send(Configuration., new JsonObject().put("frequency", this.frequency));
    }

}
