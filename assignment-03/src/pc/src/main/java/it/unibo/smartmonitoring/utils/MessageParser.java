package it.unibo.smartmonitoring.utils;

import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.model.api.SmartThermometer;
import it.unibo.smartmonitoring.model.api.SmartWindow;

public final class MessageParser {

    public static enum HTTPMessageType {

        UPDATE,
        SET_WINDOW_APERTURE,
        SET_MODE,
        RESET_ALARM;

    }

    public static HTTPMessageType getHTTPMessageType(final JsonObject message) {
        final String type = message.getString("type");
        switch (type) {
            case "update":
                return HTTPMessageType.UPDATE;
            case "set-aperture":
                return HTTPMessageType.SET_WINDOW_APERTURE;
            case "set-mode":
                return HTTPMessageType.SET_MODE;
            case "reset-alarm":
                return HTTPMessageType.RESET_ALARM;
            default:
                throw new IllegalArgumentException("Invalid message type: " + type);
        }
    }

    public static float parseMQTTMessage(final JsonObject message) {
        return message.getFloat("temperature");
    }

    /**
     * Create a JSON object to update the HTTP fronted with the current status of the thermometer and window.
     * @param thermometer
     * @param window
     * @return
     */
    public static JsonObject createHTTPUpdate(final SmartThermometer thermometer, final SmartWindow window) {
        final JsonObject json = new JsonObject();
        
        return json;
    }
    
}
