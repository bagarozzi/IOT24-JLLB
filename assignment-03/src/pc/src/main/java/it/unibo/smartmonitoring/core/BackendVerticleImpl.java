package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;

import it.unibo.smartmonitoring.core.api.BackendVerticle;

public class BackendVerticleImpl extends AbstractVerticle implements BackendVerticle {

    private State state;

    @Override
    public void start() {
        
    }

    public void update() {
        System.out.println("BackendVerticleImpl.update");
    }
    
}
