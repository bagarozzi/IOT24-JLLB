/**
 * This configuration file is used to define the configuration PINs of the system.
 */

#ifndef __CONFIG__
#define __CONFIG__

// System timing and configuration
#define SLEEP_TIMEOUT 10000
#define LID_OPEN_TIMEOUT 120000
#define TELEMETRY_TIMEOUT 500

// Temperature check task's timing configuration: 
#define TEMPERATURE_CHECK_TIMEOUT 500 // Checks every 500ms
#define TEMPERATURE_CHECK_MAXTIME 5000 // If the temp. goes "MAXTEMP" for 5 seconds
#define TEMPERATURE_CHECK_MAXTEMP 40 // The maximum temperature allowed

// PINs

#endif