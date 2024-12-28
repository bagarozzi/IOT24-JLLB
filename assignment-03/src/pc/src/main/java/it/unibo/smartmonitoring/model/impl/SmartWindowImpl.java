package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.model.api.SmartWindow;

public class SmartWindowImpl extends AbstractVerticle implements SmartWindow {

    private int angle;
    private final BackendVerticle backend;

    public SmartWindowImpl(final BackendVerticle backend) {
        this.backend = backend;
        angle = 0;
    }

    @Override
    public void start() {
        setEventBusConsumer();
    }

    @Override
    public void setAngle(final int angle) {
        this.angle = angle;
    }

    @Override
    public int getAngle() {
        return angle;
    }

    private void setAngleWithoutSending(final int angle) {
        this.angle = angle;
    }

    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_ARDUINO_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from Arduino verticle");
            /**
             * Messaggi ricevuti dalla finestra:
             * - "update-mode" -> manual or automatic
             * - "update-angle" (se in manual-mode)
             * 
             * Messaggi inviati alla finestra: 
             * - "update-temperature" -> temperatura SOLO QUANDO MANUAL MODE
             * - "set-aperture" -> apertura finestra SOLO quando in AUTOMATIC MODE
             * - "set-mode" -> manual or automatic quando viene impostato dal sito
             */
        });
    }
    
}
