package it.unibo.smartmonitoring.core;

import java.io.ObjectInputFilter.Config;

import io.vertx.core.AbstractVerticle;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;

public class BackendVerticleImpl extends AbstractVerticle implements BackendVerticle {

    private State state;
    private long stateTimestamp;

    @Override
    public void start() {
        setState(State.IDLE);
        vertx.setPeriodic(100, id -> {
            this.update();
        });
        setEventBusConsumer();
    }

    public void update() {
        switch(state) {
            case MANUAL:
                break;
            case NORMAL:
                break;
            case HOT:
                break;
            case TOO_HOT:
                break;
            case ALARM:
                break;
            case IDLE:
                break;
        }
    }
    
    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_MQTT_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from MQTT verticle");
            /* TODO: update the objects with data in the message */
        });
        vertx.eventBus().consumer(Configuration.BACKEND_HTTP_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from HTTP verticle");
            /* TODO: serve the HTTP request */
        });
        vertx.eventBus().consumer(Configuration.BACKEND_ARDUINO_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from Arduino verticle");
            /* TODO: update the objects with data in the message */
        });
    }

    private long elapsedTimeInState() {
        return System.currentTimeMillis() - stateTimestamp;
    }

    private void setState(final State state) {
        this.state = state;
        stateTimestamp = System.currentTimeMillis();
    }

    private void log(String msg) {
        System.out.println("[BACKEND]: " + msg);
    }
    
}
