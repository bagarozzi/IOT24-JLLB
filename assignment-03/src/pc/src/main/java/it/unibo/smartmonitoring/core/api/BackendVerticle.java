package it.unibo.smartmonitoring.core.api;

import io.vertx.core.Verticle;

public interface BackendVerticle extends Verticle {
    
    public enum State {
        MANUAL,
        NORMAL,
        HOT,
        TOO_HOT,
        ALARM,
        IDLE
    }

    /** 
     * Starts the Verticle and initializes the Finite State Machine.
     */
    public void start();

    /**
     * Updates the state of the Finite State Machine controlling the backend.
     */
    public void update();
}
