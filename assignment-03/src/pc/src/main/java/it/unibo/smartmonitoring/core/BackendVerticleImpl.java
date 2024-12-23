package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;

import it.unibo.smartmonitoring.core.api.BackendVerticle;

public class BackendVerticleImpl extends AbstractVerticle implements BackendVerticle {

    private State state;
    private long stateTimestamp;

    @Override
    public void start() {
        setState(State.IDLE);
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

    private long elapsedTimeInState() {
        return System.currentTimeMillis() - stateTimestamp;
    }

    private void setState(final State state) {
        this.state = state;
        stateTimestamp = System.currentTimeMillis();
    }
    
}
