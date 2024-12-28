package it.unibo.smartmonitoring.model.api;

import io.vertx.core.Verticle;

public interface SmartWindow extends Verticle {
    
    /** 
     * Starts the Verticle and initializes the smart window verticle.
     */
    public void start();

    /** 
     * Sets and sends the angle of the window aperture to the Arduino verticle.
     */
    public void setAngle(int angle);

    /**
     * Returns the angle of the window aperture.
     * @return the aperture angle in percentage.
     */
    public int getAngle();

    /**
     * Sends the temperature update over to the Arduino verticle. To be used when the 
     * Arduino is in manual mode.
     */
    public void sendTemperatureUpdate(float temperature);

    /**
     * Sends the state of the system to the Arduino verticle.
     */
    public void sendStateUpdate(String state);

}
