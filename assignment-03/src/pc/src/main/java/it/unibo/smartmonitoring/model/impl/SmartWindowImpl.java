package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.model.api.SmartWindow;

public class SmartWindowImpl extends AbstractVerticle implements SmartWindow {

    @Override
    public void start() {
        setEventBusConsumer();
    }

    @Override
    public void setAngle(int angle) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'setAngle'");
    }

    @Override
    public int getAngle() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getAngle'");
    }

    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_ARDUINO_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from Arduino verticle");
            /* TODO: update the objects with data in the message */
        });
    }
    
}
