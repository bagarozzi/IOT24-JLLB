package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.model.api.SmartWindow;

public class SmartWindowImpl extends AbstractVerticle implements SmartWindow {

    private int angle;

    @Override
    public void start() {
        setEventBusConsumer();
    }

    @Override
    public void setAngle(int angle) {
        this.angle = angle;
    }

    @Override
    public int getAngle() {
        return angle;
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
