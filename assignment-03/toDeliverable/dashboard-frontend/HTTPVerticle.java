package it.unibo.smartmonitoring.core;

import java.util.ArrayList;
import java.util.List;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.StaticHandler;
import it.unibo.smartmonitoring.Configuration;

public class HTTPVerticle extends AbstractVerticle {

	// Stato del sistema simulato
	private SystemState systemState;

	@Override
	public void start() {
		systemState = new SystemState();
		// Inizializza lo stato del sistema
		setEventBusConsumer();

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
			vertx.eventBus().request(
				Configuration.BACKEND_HTTP_EB_ADDR,
				new JsonObject().put("type", "update"),
				msg -> {
					JsonObject body = (JsonObject) msg.result().body();
					systemState.setTemperature(body.getDouble("temperature"));
					systemState.setMinTemperature(body.getDouble("minTemperature"));
					systemState.setMaxTemperature(body.getDouble("maxTemperature"));
          systemState.setAverageTemperature(body.getFloat("averageTemperature"));
					systemState.setMode(body.getString("mode"));
					systemState.setWindowOpening(body.getInteger("windowOpening"));
					systemState.setSystemState(body.getString("systemState"));
				}

			);
			ctx.json(systemState.toJson());
		});

		// Endpoint: Imposta l'apertura manuale della finestra
		router.post("/api/manual-mode").handler(ctx -> {
			if(systemState.mode.equals("manual")) {

				JsonObject body = ctx.body().asJsonObject();
				int windowOpening = body.getInteger("windowOpening", 0);

				// Aggiorna lo stato del sistema
				systemState.setWindowOpening(windowOpening);

				ctx.json(new JsonObject().put("status", "success"));
				vertx.eventBus().send(
					Configuration.BACKEND_HTTP_EB_ADDR,
					new JsonObject().put("type", "set-aperture").put("aperture", windowOpening)
				);
			}
		});

		// Endpoint: Risolvi stato di allarme
		router.post("/api/resolve-alarm").handler(ctx -> {
			// Cambia lo stato a normal
			systemState.setSystemState("normal");
			ctx.json(new JsonObject().put("status", "success"));
			vertx.eventBus().send(
				Configuration.BACKEND_HTTP_EB_ADDR,
				new JsonObject().put("type", "reset-alarm")
			);
		});

		// Endpoint: Cambia modalità del sistema
		router.post("/api/switch-mode").handler(ctx -> {
			JsonObject body = ctx.body().asJsonObject();
			String newMode = body.getString("mode", "auto"); // Default to AUTOMATIC
			systemState.setMode(newMode);
			ctx.json(new JsonObject().put("status", "success").put("newMode", newMode));
			vertx.eventBus().send(
				Configuration.BACKEND_HTTP_EB_ADDR,
				new JsonObject().put("type", "set-mode").put("mode", newMode)
			);
		});

		// Avvia il server HTTP
		vertx.createHttpServer()
			.requestHandler(router)
			.listen(8082, result -> {
				if (result.succeeded()) {
					log("Server avviato su http://localhost:8082");
				} else {
					log("Errore nell'avvio del server: " + result.cause());
				}
			});
	}


	private void setEventBusConsumer() {
		vertx.eventBus().consumer(Configuration.HTTP_EB_ADDR, message -> {
			JsonObject body = (JsonObject) message.body();
      double temperature = body.getDouble("temperature");
			systemState.setTemperature(temperature);
			systemState.setMinTemperature(body.getDouble("minTemperature"));
			systemState.setMaxTemperature(body.getDouble("maxTemperature"));
      systemState.setAverageTemperature(body.getDouble("averageTemperature"));
			systemState.setMode(body.getString("mode"));
			systemState.setWindowOpening(body.getInteger("windowOpening"));
			systemState.setSystemState(body.getString("systemState"));

      systemState.addTemperatureMeasurement(temperature);
		});
	}

	private void log(String message) {
		System.out.println("[HTTP-SERVER]: " + message);
	}

	//add average temperature
	private class SystemState {
		private double temperature;
		private double minTemperature;
		private double maxTemperature;
		private String mode;
		private int windowOpening;
		private String systemState;
    private double averageTemperature;
    private final int MAX_MEASUREMENTS = 20;
    private final List<Double> temperatureHistory = new ArrayList<>();

		public void setTemperature(double temperature) {
			this.temperature = temperature;
		}

		public void setMinTemperature(double minTemperature) {
			this.minTemperature = minTemperature;
		}

		public void setMaxTemperature(double maxTemperature) {
			this.maxTemperature = maxTemperature;
		}

		public void setMode(String mode) {
			this.mode = mode;
		}

		public void setWindowOpening(int windowOpening) {
			this.windowOpening = windowOpening;
		}

		public void setSystemState(String systemState) {
			this.systemState = systemState;
		}

    public void setAverageTemperature(double averageTemperature) {
      this.averageTemperature = averageTemperature;
    }

		//metodo che trasforma il SystemState in un JsonObject
		public JsonObject toJson() {
			return new JsonObject()
				.put("temperature", temperature)
				.put("minTemperature", minTemperature)
				.put("maxTemperature", maxTemperature)
        .put("averageTemperature", averageTemperature)
        .put("temperatureHistory", getTemperatureHistoryAsJson())
				.put("mode", mode)
				.put("windowOpening", windowOpening)
				.put("systemState", systemState);
		}

    public void addTemperatureMeasurement(double temperature) {
      // Aggiungi il valore allo storico
      if (temperatureHistory.size() >= MAX_MEASUREMENTS) {
        temperatureHistory.remove(0); // Rimuovi il valore più vecchio
      }
      temperatureHistory.add(temperature);
    }

    public JsonArray getTemperatureHistoryAsJson() {
      return new JsonArray(temperatureHistory);
    }
  }

}
