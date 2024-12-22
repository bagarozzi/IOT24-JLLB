package it.unibo.smartmonitoring.core.api;

import io.vertx.core.Verticle;

public interface BackendVerticle extends Verticle {
    
    public enum State {
        IDLE,
        BUSY
    }

    public void start();

    public void update();
}
