package it.unibo.smartmonitoring.model.api;

import io.vertx.core.Verticle;

public interface SmartWindow extends Verticle {
    
    /** 
     * Starts the Verticle and initializes the smart window verticle.
     */
    public void start();

    public void setAngle(int angle);

    public int getAngle();

}
