package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;

import it.unibo.smartmonitoring.model.api.SmartThermometer;

public class SmartThermometerImpl extends AbstractVerticle implements SmartThermometer {

    private float temperature;
    private int frequency;

    @Override
    public void start() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'start'");
    }

    @Override
    public void setFrequency(int frequency) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'setFrequency'");
    }

    @Override
    public float getTemperature() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getTemperature'");
    }

    @Override
    public void setTemperature(float temperature) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'setTemperature'");
    }

}
