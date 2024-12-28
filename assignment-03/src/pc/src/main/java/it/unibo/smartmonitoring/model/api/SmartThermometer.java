package it.unibo.smartmonitoring.model.api;

import io.vertx.core.Verticle;

public interface SmartThermometer extends Verticle {

    public void setFrequency(int frequency);

    public void setTemperature(float temperature);

    public float getTemperature();

}