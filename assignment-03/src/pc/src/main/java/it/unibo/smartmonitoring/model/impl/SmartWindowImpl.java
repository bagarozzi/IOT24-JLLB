package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.Json;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.core.api.BackendVerticle.State;
import it.unibo.smartmonitoring.model.api.SmartWindow;
import it.unibo.smartmonitoring.utils.MessageParser;

public class SmartWindowImpl extends AbstractVerticle implements SmartWindow {

    private int angle;
    private final BackendVerticle backend;

    public SmartWindowImpl(final BackendVerticle backend) {
        this.backend = backend;
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
            JsonObject body = (JsonObject) message.body();
            switch (MessageParser.getArduinoMessageType(body)) {
                case SET_MODE:
                    if(body.getString("mode").equals("manual")) {
                        backend.setManualMode();
                    } 
                    else if(body.getString("mode").equals("auto")){
                        backend.setAutomaticMode();
                    }
                    break;
                case SET_ANGLE:
                    if(backend.isState(State.MANUAL)) {
                        setAngleWithoutSending(body.getInteger("angle"));
                    }
                    break;
                default:
                    break;
            }
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
