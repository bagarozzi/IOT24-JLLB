package it.unibo.smartmonitoring.core;

import it.unibo.smartmonitoring.Configuration;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import it.unibo.smartmonitoring.serial.impl.SerialCommChannel;

public class SerialVerticle extends AbstractVerticle {

    private final SerialCommChannel channel;

    private static final String CONTAINER_PREFIX = "cw:";
    private static final String LOG_PREFIX = "lo:";
	private static final String MSG_STATE = "st:";

    public SerialVerticle(SerialCommChannel channel) throws Exception {
        this.channel = channel;
    }

    @Override
    public void start() {
        /**
         * Event bus consumer to receive messages from the backend verticle and send them to the serial channel.
         */
        vertx.eventBus().consumer(Configuration.ARUDINO_EB_ADDR, message -> {
            JsonObject json = (JsonObject) message.body();
            switch (json.getString("type")) {
                case "update-angle":
                    channel.sendMsg(CONTAINER_PREFIX + "angle:" + json.getInteger("angle"));
                    break;
                case "update-mode":
                    channel.sendMsg(CONTAINER_PREFIX + "mode:" + (json.getString("mode").equals("auto") ? 0 : 1));
                    break;
                case "update-temperature":
                    channel.sendMsg(CONTAINER_PREFIX + "temperature:" + json.getFloat("temperature"));
                    break;
                default:
                    break;
            }
        });
        vertx.setPeriodic(100, id -> {
            this.update();
        });
    }

    public void update() {
        try {
            processMsg();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * Process the message received from the serial channel and send it to the backend verticle.
    * @throws InterruptedException
    */
    private void processMsg() throws InterruptedException {
        String msg = channel.receiveMsg();
        if (msg.startsWith(CONTAINER_PREFIX)){
            String cmd = msg.substring(CONTAINER_PREFIX.length());             
            if (cmd.startsWith(MSG_STATE)){
                String args = cmd.substring(MSG_STATE.length()); 
                String[] elems = args.split(":");
                if (elems.length >= 2) {
                    String type = elems[0];
                    int value = Integer.parseInt(elems[1]);
                    if (type == "mode") {
                        JsonObject json = new JsonObject()
                            .put("type", "update-mode")
                            .put("mode", value == 0 ? "auto" : "manual");
                        vertx.eventBus().send(Configuration.BACKEND_ARDUINO_EB_ADDR, json);
                    } else if (type == "angle") {
                        JsonObject json = new JsonObject()
                            .put("type", "update-angle")
                            .put("angle", value);
                        vertx.eventBus().send(Configuration.BACKEND_ARDUINO_EB_ADDR, json);
                    }
                } 
            }
        } else if (msg.startsWith(LOG_PREFIX)){
            System.out.println("[SERIAL]: " + msg.substring(LOG_PREFIX.length()));
        }
    }
}
