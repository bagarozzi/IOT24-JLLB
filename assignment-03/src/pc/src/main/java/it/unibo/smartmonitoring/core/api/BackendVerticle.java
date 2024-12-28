package it.unibo.smartmonitoring.core.api;

import io.vertx.core.Verticle;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.model.api.SmartWindow;

public interface BackendVerticle extends Verticle {
    
    public enum State {
        /**
         * The system's manual mode, the Arduino component is controlling
         * the aperture.s
         */

        MANUAL,
        /**
         * The system is in normal mode. The temperature is less than
         * {@link Configuration#NORMAL_MODE_THRESHOLD}.
         */
        NORMAL,

        /**
         * The system is in hot mode. The temperature is between NORMAL_MODE_THRESHOLD
         * and HOT_MODE_THRESHOLD so the windows will be opened proportionally to the
         * temperature.
         * From now on the system starts monitoring the temperature more frequently.
         */
        HOT,

        /** 
         * The system is in too hot mode. The window is fully open.
         */
        TOO_HOT,

        /**
         * The system was for too much time in the TOO_HOT state so now it's in alarm.
         */
        ALARM,

        /**
         * The state where the system is spawned and where it comes back after ending
         * manual mode. 
         */
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
     * Sets the system to automatic mode.
     */
    public void setAutomaticMode();

    /**
     * Sets the system to manual mode.
     */
    public void setManualMode();

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
