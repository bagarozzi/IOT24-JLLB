#include "TempSensorLM35.h"
#include "Arduino.h"

#define ADC_WIDTH ADC_WIDTH_BIT_12
#define ADC_ATTEN ADC_ATTEN_DB_12
#define DEFAULT_VREF 1100
#define VCC ((float)3.3)

TempSensorLM35::TempSensorLM35(adc1_channel_t adcChannel, int pin) : adcChannel(adcChannel), pin(pin)
{
	adc1_config_width(ADC_WIDTH_BIT_12); // set the adc1 withd to vale from 0 to 4095

	adc1_config_channel_atten(adcChannel, ADC_ATTEN); //set the attenuation of the adc channel for tension up to 3.3V
	uint32_t adc_reading = adc1_get_raw(ADC1_CHANNEL_0);
  	float vref = (1.1 * 4095.0) / adc_reading;
    adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, vref, adc_chars);
	pinMode(pin, INPUT);
}

float TempSensorLM35::getTemperature()
{
	float values[5];
	float max = -1;
	float min = 100;

	/*
	   simple strategy for input conditioning:
	   - doing multiple measurements, discarding mix and max and returning the average
	*/

	for (int i = 0; i < 5; i++)
	{
		uint32_t raw_adc = adc1_get_raw(adcChannel); // Use adc1_get_raw for more precise readings
        uint32_t voltage = esp_adc_cal_raw_to_voltage(raw_adc, adc_chars); // cConvert into tension
		float valueInCelsius = (float)voltage / 10.0; // LM35: 10 mV/°C
		values[i] = valueInCelsius;
		if (valueInCelsius < min)
		{
			min = valueInCelsius;
		}
		else if (valueInCelsius > max)
		{
			max = valueInCelsius;
		}
	}
	float sum = 0;
	int count = 0;
	for (int i = 0; i < 5; i++)
	{
		if ((values[i] > min) && (values[i] < max))
		{
			sum = sum + values[i];
			count++;
		}
	}
	if (count == 0)
	{
		return values[0];
	}
	else
	{
		return sum / count;
	}
}
