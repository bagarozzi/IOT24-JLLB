package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;

public class SerialVerticle extends AbstractVerticle {

    @Override
    public void start() {
        vertx.eventBus().consumer("serial", message -> {
            String data = (String) message.body();
            System.out.println("Received data: " + data);
        });
    }
    /*
     * Inviare dati a arduino: codice(Automatico o manuale):livelloFinestra(%):temperatura
     */
    
}
