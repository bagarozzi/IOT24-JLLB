package it.unibo.smartmonitoring.core;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Promise;

import it.unibo.smartmonitoring.core.BackendVerticleImpl;
import it.unibo.smartmonitoring.core.api.BackendVerticle;

public class MainVerticle extends AbstractVerticle {

  @Override
  public void start(Promise<Void> startPromise) throws Exception {
	BackendVerticle backendVerticle = new BackendVerticleImpl();
	vertx.deployVerticle(backendVerticle);
  }
}
