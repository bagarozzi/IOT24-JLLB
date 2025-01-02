package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.StaticHandler;

public class HTTPVerticle extends AbstractVerticle {

    // Stato del sistema simulato
    private JsonObject systemState;

    @Override
    public void start() {
        // Inizializza lo stato del sistema
        initSystemState();

        // Configura il router
        Router router = Router.router(vertx);

        // Static files handler (serve frontend)
        router.route("/static/*").handler(StaticHandler.create("src/resources/webroot"));

        // Body handler per richieste POST
        router.route().handler(BodyHandler.create());

        router.get("/").handler(ctx -> {
            ctx.response()
                .putHeader("Content-Type", "text/html")
                .sendFile("src/resources/webroot/index.html");
        });

        // Endpoint: Ottieni lo stato del sistema
        router.get("/api/system-state").handler(ctx -> {
            ctx.json(systemState);
        });

        // Endpoint: Imposta l'apertura manuale della finestra
        router.post("/api/manual-mode").handler(ctx -> {
            JsonObject body = ctx.body().asJsonObject();
            int windowOpening = body.getInteger("windowOpening", 0);

            // Aggiorna lo stato del sistema
            systemState.put("windowOpening", windowOpening);
            systemState.put("mode", "MANUAL"); // Cambia la modalità a MANUAL

            ctx.json(new JsonObject().put("status", "success"));
        });

        // Endpoint: Risolvi stato di allarme
        router.post("/api/resolve-alarm").handler(ctx -> {
            // Cambia lo stato a NORMAL
            systemState.put("systemState", "NORMAL");
            ctx.json(new JsonObject().put("status", "success"));
        });

        // Endpoint: Cambia modalità del sistema
        router.post("/api/switch-mode").handler(ctx -> {
            JsonObject body = ctx.body().asJsonObject();
            String newMode = body.getString("mode", "AUTOMATIC"); // Default to AUTOMATIC
            systemState.put("mode", newMode);
            ctx.json(new JsonObject().put("status", "success").put("newMode", newMode));
        });

        // Avvia il server HTTP
        vertx.createHttpServer()
            .requestHandler(router)
            .listen(8081, result -> {
                if (result.succeeded()) {
                    log("Server avviato su http://localhost:8081");
                } else {
                    log("Errore nell'avvio del server: " + result.cause());
                }
            });
    }

    // Inizializza lo stato simulato del sistema
    private void initSystemState() {
        systemState = new JsonObject()
            .put("temperature", 25.4)
            .put("minTemperature", 20.0)
            .put("maxTemperature", 30.0)
            .put("mode", "AUTOMATIC")
            .put("windowOpening", 0) // Finestra inizialmente chiusa
            .put("systemState", "NORMAL");
    }

	private void log(String message) {
		System.out.println("[HTTP-SERVER]: " + message);
	}
}
