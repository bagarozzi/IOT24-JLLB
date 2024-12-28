package it.unibo.smartmonitoring.model.api;

import io.vertx.core.Verticle;

public interface SmartThermometer extends Verticle {

    /** 
     * Starts the Verticle and initializes the smart thermometer verticle.
     */
    public void start();

    public void setFrequency(int frequency);

    public void setTemperature(float temperature);

    public float getTemperature();

}