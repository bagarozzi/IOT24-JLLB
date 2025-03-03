package it.unibo.smartmonitoring.model.impl;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.Configuration;
import it.unibo.smartmonitoring.core.api.BackendVerticle;
import it.unibo.smartmonitoring.core.api.BackendVerticle.State;
import it.unibo.smartmonitoring.model.api.SmartDashboard;
import it.unibo.smartmonitoring.utils.MessageParser;

public class SmartDashboardImpl extends AbstractVerticle implements SmartDashboard {

    private final BackendVerticle backend;

    public SmartDashboardImpl(final BackendVerticle backend) {
        this.backend = backend;
    }

    @Override
    public void start() {
        setEventBusConsumer();
        System.out.println("[BACKEND]: SmartDashboard deployment completed");
    }

    @Override
    public void sendDashboardUpdate() {
        vertx.eventBus().send(
            Configuration.HTTP_EB_ADDR, 
            MessageParser.createHTTPUpdate(backend.getSmartThermometer(), backend.getSmartWindow(), backend)  
        );
    }

    private void setEventBusConsumer() {
        vertx.eventBus().consumer(Configuration.BACKEND_HTTP_EB_ADDR, message -> {
            System.out.println("[BACKEND]: Received message from HTTP verticle");
            JsonObject body = (JsonObject) message.body();
            switch (MessageParser.getHTTPMessageType(body)) {
                case UPDATE:
                    message.reply(MessageParser.createHTTPUpdate(backend.getSmartThermometer(), backend.getSmartWindow(), backend));
                    break;
                case RESET_ALARM:
                    backend.resetAlarm();
                    break;
                case SET_MODE:
                    if(body.getString("mode").equals("manual")) {
                        backend.setManualMode(State.MANUAL_DASHBOARD);
                    } 
                    else if(body.getString("mode").equals("auto")){
                        backend.setAutomaticMode();
                    }
                    break;
                case SET_WINDOW_APERTURE:
                    backend.setWindowAperture(body.getInteger("aperture"));
                    break;
                default:
                    break;
            }
        });
    }
    
}
