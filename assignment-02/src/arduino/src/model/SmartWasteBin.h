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
        void userGone(); // sets the state to USER_GONE
        bool isUserGone(); // checks if the state is USER_GONE
        void readyToOpen(); // sets the state to READY_TO_OPEN (called by the button pressed)
        bool isReadyToOpen(); // checks if the state is READY_TO_OPEN

        // Methods for disposing:
        void openBin(); // opens the bin with the servo
        void openingCompleted(); // sets the state to OPEN
        bool isBinOpen(); // checks if the state is OPEN
        void readyForDisposal(); // sets the state to READY_FOR_DISPOSAL
        bool isReadyForDisposal(); // checks if the state is READY_FOR_DISPOSAL
        void disposalCompleted(); // sets the state to DISPOSAL_COMPLETED
        bool isDisposingDone(); // checks if the state is DISPOSAL_COMPLETED

        void readyToClose(); // sets the state to READY_TO_CLOSE
        bool isReadyToClose(); // checks if the state is READY_TO_CLOSE
        void closeBin(); // closes the bin with the servo
        void closingCompleted(); // sets the state to CLOSED
        bool isBinClosed(); // checks if the state is CLOSED

        bool isFull(); // checks if the bin is full (getCurrentLevel() > MAX_LEVEL)

        // Methods for maintenance:
        void problemDetected(); // sets the state to MAINTENANCE
        void setActuatorsInMaintenance(); // sets the system in maintenance mode, red light and closes lid
        bool isInMaintenance();
        bool isMaintenanceCompleted();

        // Methods for sleeping
        void prepareForSleep();
        void wakeUp();

        // Methods for interacting with sensors and actuators: 
        double getCurrentLevel();
        double getCurrentTemperature();

        void sampleTemperature();
        void sampleDistance();
        void samplePresence();

    private:

        enum {
            IDLE,
            USER_DETECTED,
            USER_GONE,
            READY_TO_OPEN,
            OPENED,
            READY_FOR_DISPOSAL,
            DISPOSAL,
            DISPOSAL_COMPLETED,
            READY_TO_CLOSE,
            CLOSED,
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
