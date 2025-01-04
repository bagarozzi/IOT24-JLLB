package it.unibo.smartmonitoring.model.api;

import io.vertx.core.Verticle;

public interface SmartDashboard extends Verticle {

    public void sendDashboardUpdate();
    
}
