#ifndef __SMART_WASTE_BIN__
#define __SMART_WASTE_BIN__

#include <Arduino.h>
#include "Led.h"
#include "Sonar.h"
#include "Pir.h"
#include "TempSensor.h"
#include "servo_motor.h"

class SmartWasteBin {

    public:

        SmartWasteBin(); 
        void init();

        // Methods for user detection
        void userDetected();
        bool isUserDetected();
        void userGone();
        bool isUserGone();
        void readyToOpen();
        bool isReadyToOpen();

        // Methods for disposing:
        void binOpen();
        bool isBinOpen();
        bool isDisposingDone();

        // Methods for maintenance:
        void problemDetected(); // sets the state to MAINTENANCE
        void setActuatorsInMaintenance(); // sets the system in maintenance mode, red light and closes lid
        bool isInMaintenance();
        bool isMaintenanceCompleted();

        // Methods for sleeping
        void prepareForSleep();
        void wakeUp();

        // Methods for interacting with sensors and actuators: 
        bool sampleTemperature();
        bool sampleDistance();
        bool samplePresence();

    private:

        enum {
            IDLE,
            USER_DETECTED,
            USER_GONE,
            READY_TO_OPEN,
            OPEN,
            MAINTENANCE
        } state; 

        Led* Led1;
        Led* Led2;

        Sonar* Sonar;
        Pir* Pir;
        TempSensor* TempSensor;
        ServoMotor* CoverMotor;  
};

#endif
