package it.unibo.smartmonitoring.core.api;

import io.vertx.core.Verticle;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.model.api.SmartWindow;

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

    /**
     * Returns wether the state is the one passed as parameter.
     * @return
     */
    public boolean isState(State state);

    /**
     *  Returns the SmartThermometer instance related to this backend.
     */
    public SmartThermometer getSmartThermometer();

    /** 
     * Returns the SmartWindow instance related to this backend.
     */
    public SmartWindow getSmartWindow();
}
