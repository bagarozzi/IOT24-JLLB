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

    public void start();

    public void update();
}
