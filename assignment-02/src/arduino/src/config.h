/**
 * This configuration file is used to define the configuration PINs of the system.
 */

#ifndef __CONFIG__
#define __CONFIG__

// Scheduler's timing configuration:
#define BASE_PERIOD 100

// System timing and configuration:
#define SLEEP_TIMEOUT 10000
#define LID_OPEN_TIMEOUT 120000
#define MESSAGE_CLOSING_TIMEOUT 3000
#define MAINTENANCE_TIMEOUT 20000
#define BIN_CLOSING_TIME 500

// Telemtry task's timing configuration:
#define TELEMETRY_TIMEOUT 600

// Temperature check task's timing configuration: 
#define TEMPERATURE_CHECK_TIMEOUT 600 // Checks every 600ms
#define TEMPERATURE_CHECK_MAXTIME 5100 // If the temp. goes "MAXTEMP" for 5.1 seconds or more 
#define TEMPERATURE_CHECK_MAXTEMP 79 // The maximum temperature allowed

// User detection task's timing configuration:
#define DETECTION_TIMEOUT 400 // checks every 400ms

// Waste bin level check:
#define MAX_LEVEL 0.05f // The maximum level of the waste bin (95%)
#define BIN_CAPACITY 1.0f // The minimum level of the waste bin (1 meter)

// PINs configuration:
#define LED1_PIN 3 //green led
#define LED2_PIN 8 //red led
#define SONAR_ECHO_PIN 10
#define SONAR_TRIG_PIN 9
#define PIR_PIN 2
#define TEMP_SENSOR_PIN A0
#define SERVO_PIN 11
#define OPEN_BUTTON_PIN 7
#define CLOSE_BUTTON_PIN 6

//TODO: Aggiungi servo apertura operatore
#define SERVO_CLOSE_ANGLE 90
#define SERVO_OPEN_ANGLE 0
#define SERVO_MAINTENANCE_ANGLE 180

#endif