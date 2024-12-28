package it.unibo.smartmonitoring.model.api;

public interface SmartThermometer {

    public void setFrequency(int frequency);

    public void setTemperature(float temperature);

    public float getTemperature();

}