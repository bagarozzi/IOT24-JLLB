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

// Telemtry task's timing configuration:
#define TELEMETRY_TIMEOUT 600

// Temperature check task's timing configuration: 
#define TEMPERATURE_CHECK_TIMEOUT 600 // Checks every 500ms
#define TEMPERATURE_CHECK_MAXTIME 5000 // If the temp. goes "MAXTEMP" for 5 seconds
#define TEMPERATURE_CHECK_MAXTEMP 40 // The maximum temperature allowed

// Waste bin level check:
#define MAX_LEVEL 10 // The maximum level of the waste bin

// PINs configuration:
#define LED1_PIN 3
#define LED2_PIN 2
#define SONAR_ECHO_PIN 9
#define SONAR_TRIG_PIN 10
#define PIR_PIN A3
#define TEMP_SENSOR_PIN A0

#define SERVO_CLOSE_ANGLE 0
#define SERVE_OPEN_ANGLE 90

#endif