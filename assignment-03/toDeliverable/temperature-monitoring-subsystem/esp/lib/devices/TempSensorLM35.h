#ifndef __TEMP_SENSOR_LM35__
#define __TEMP_SENSOR_LM35__

#include "TempSensor.h"
#include "esp_adc_cal.h"

class TempSensorLM35: public TempSensor {
public:
  TempSensorLM35(adc1_channel_t adcChannel, int pin);
  virtual float getTemperature();
private:
  adc1_channel_t adcChannel;
  int pin;
  float vref ;
  esp_adc_cal_characteristics_t * adc_chars;
};


#endif
