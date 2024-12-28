package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.model.impl.SmartThermometerImpl;
import it.unibo.smartmonitoring.utils.MessageParser;

public class BackendVerticleImpl extends AbstractVerticle implements BackendVerticle {

    private State state;
    private long stateTimestamp;
    private boolean justEnteredState;

    private final SmartThermometer thermometer;

    public BackendVerticleImpl() {
        setState(State.IDLE);
        thermometer = new SmartThermometerImpl();
        stateTimestamp = System.currentTimeMillis();
        justEnteredState = false;
    }

    @Override
    public void start() {
        vertx.setPeriodic(100, id -> {
            this.update();
        });
        setEventBusConsumer();
        log("deployment completed");
    }

    public void update() {
        switch(state) {
            case MANUAL:
                logOnce("state MANUAL");
                break;
            case NORMAL:
                logOnce("state NORMAL");
                break;
            case HOT:
                logOnce("state HOT");
                break;
            case TOO_HOT:
                logOnce("state TOO HOT");
                break;
            case ALARM:
                logOnce("state ALARM");
                break;
            case IDLE:
                logOnce("state IDLE");
                break;
        }
    }
    
    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_MQTT_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from MQTT verticle");
            thermometer.setTemperature(MessageParser.parseMQTTMessage((JsonObject)message.body()));
        });
        vertx.eventBus().consumer(Configuration.BACKEND_HTTP_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from HTTP verticle");
            switch (MessageParser.getHTTPMessageType((JsonObject)message.body())) {
                case UPDATE:
                    message.reply(MessageParser.createHTTPUpdate(null, null));
                    break;
                case RESET_ALARM:
                    break;
                case SET_MODE:
                    setState(State.MANUAL);
                    break;
                case SET_WINDOW_APERTURE:
                    break;
                default:
                    break;
            }
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
        justEnteredState = true;
        stateTimestamp = System.currentTimeMillis();
    }

    private void log(String msg) {
        System.out.println("[BACKEND]: " + msg);
    }

    private void logOnce(String msg) {
        System.out.println("[BACKEND]: " + msg);
        justEnteredState = false;
    }
    
}
