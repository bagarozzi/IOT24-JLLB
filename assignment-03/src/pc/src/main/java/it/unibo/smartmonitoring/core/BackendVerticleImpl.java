package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.model.api.SmartDashboard;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.model.api.SmartWindow;
import it.unibo.smartmonitoring.model.impl.SmartDashboardImpl;
import it.unibo.smartmonitoring.model.impl.SmartThermometerImpl;
import it.unibo.smartmonitoring.model.impl.SmartWindowImpl;
import it.unibo.smartmonitoring.utils.MessageParser;

public class BackendVerticleImpl extends AbstractVerticle implements BackendVerticle {

    private State state;
    private long stateTimestamp;
    private boolean justEnteredState;

    private final SmartThermometer thermometer;
    private final SmartWindow window;
    private final SmartDashboard dashboard;

    public BackendVerticleImpl() {
        thermometer = new SmartThermometerImpl(this);
        window = new SmartWindowImpl(this);
        dashboard = new SmartDashboardImpl(this);
    }

    @Override
    public void start() {
        vertx.deployVerticle(window);
        vertx.deployVerticle(thermometer);
        vertx.deployVerticle(dashboard);
        vertx.setPeriodic(100, id -> {
            this.update();
        });
        setState(State.IDLE);
        log(" Backend verticle deployment completed");
    }

    public void update() {
        float t = thermometer.getTemperature();
        switch(state) {
            case MANUAL:
                logOnce("state MANUAL");
                /**
                 * Do nothing. Updating the values is handled by the HTTP verticle
                 * itself.
                 */
                // TODO: QUANDO IL SITO METTE IN MANUALE BISOGNA DIRLO ALL'ARDUINO
                window.sendTemperatureUpdate(t);
                break;
            case NORMAL:
                logOnce("state NORMAL");
                window.setAngle(0);
                thermometer.setFrequency(Configuration.NORMAL_MODE_POLLING_FREQUENCY);
                if(t > Configuration.NORMAL_MODE_THRESHOLD) {
                    log("temperature is above normal threshold");
                    setState(State.HOT);
                    break;
                }
                break;
            case HOT:
                logOnce("state HOT");
                thermometer.setFrequency(Configuration.HOT_MODE_POLLING_FREQUENCY);
                if(t < Configuration.NORMAL_MODE_THRESHOLD) {
                    log("temperature is back below hot threshold");
                    setState(State.NORMAL);
                    break;
                }
                else if(t > Configuration.HOT_MODE_THRESHOLD) {
                    log("temperature is above hot threshold");
                    setState(State.TOO_HOT);
                    break;
                }
                else {
                    window.setAngle(computeWindowAperture(t));
                }
                break;
            case TOO_HOT:
                logOnce("state TOO HOT");
                window.setAngle(100);
                if(t < Configuration.HOT_MODE_THRESHOLD) {
                    log("temperature is back below too hot threshold");
                    setState(State.HOT);
                    break;
                }
                else if(elapsedTimeInState() >= Configuration.ALARM_THRESHOLD_TIME) {
                    log("alarm threshold time reached!");
                    setState(State.ALARM);
                    break;
                }
                break;
            case ALARM:
                logOnce("state ALARM");
                break;
            case IDLE:
                window.setAngle(0);
                thermometer.setFrequency(Configuration.NORMAL_MODE_POLLING_FREQUENCY);
                logOnce("state IDLE");
                if(t < Configuration.NORMAL_MODE_THRESHOLD) {
                    setState(State.NORMAL);
                    break;
                }
                else if(t > Configuration.NORMAL_MODE_THRESHOLD && t < Configuration.HOT_MODE_THRESHOLD) {
                    setState(State.HOT);
                    break;
                }
                else if(t > Configuration.HOT_MODE_THRESHOLD) {
                    setState(State.TOO_HOT);
                    break;
                }
                break;
        }
    }

    @Override
    public void setAutomaticMode() {
        setState(State.IDLE);
    }

    @Override
    public void setManualMode() {
        setState(State.MANUAL);
    }

    @Override
    public void resetAlarm() {
        setState(State.IDLE);
    }

    @Override
    public void setWindowAperture(final int angle) {
        if(isState(State.MANUAL)) {
            window.setAngle(angle);
        }
    }

    @Override
    public SmartThermometer getSmartThermometer() {
        return thermometer;
    }

    @Override
    public SmartWindow getSmartWindow() {
        return window;
    }

    @Override
    public boolean isState(State state) {
        return this.state == state;
    }

    private void setState(final State state) {
        if(!isState(state)) {
            this.state = state;
            justEnteredState = true;
            stateTimestamp = System.currentTimeMillis();
        }
    }

    private long elapsedTimeInState() {
        return System.currentTimeMillis() - stateTimestamp;
    }

    private void log(String msg) {
        System.out.println("[BACKEND]: " + msg);
    }

    private void logOnce(String msg) {
        System.out.println("[BACKEND]: " + msg);
        justEnteredState = false;
    }

    private int computeWindowAperture(final float temperature) {
        float deltaInterval = Configuration.HOT_MODE_THRESHOLD - Configuration.NORMAL_MODE_THRESHOLD;
        float deltaTemperature = temperature - Configuration.NORMAL_MODE_THRESHOLD;
        return (int) (deltaTemperature / deltaInterval * 100);
    }
    
}
