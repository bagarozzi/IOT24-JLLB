#ifndef __SMART_WASTE_BIN__
#define __SMART_WASTE_BIN__

#include <Arduino.h>
#include "Led.h"
#include "Sonar.h"
#include "Pir.h"
#include "TempSensorLM35.h"
#include "servo_motor_impl.h"
#include <avr/sleep.h>

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
        bool isIdle(); // checks if the state is IDLE
        void setIdle();
        
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
        void setActuatorsInMaintenance(); // powers off the green light, turns on the red light
        void setActuatorsInIdle(); // powers off the red light, turns on the green light
        bool isInMaintenance();
        bool isMaintenanceCompleted();
        void openBinForEmptying();
        void maintenanceCompleted();
        
        // Methods for sleeping
        void enableSleepInterrpt();
        void disableSleepInterrpt();

        // Methods for interacting with sensors and actuators: 
        double getCurrentLevel();
        double getCurrentTemperature();
        bool sampleUserPresence();
        
    private:

        enum State {
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
            MAINTENANCE,
            MAINTENANCE_COMPLETED
        } state; 

        void setState(State state);

        Led* led1; // Green LED
        Led* led2; // Red LED

        Sonar* sonar;
        Pir* pir;
        TempSensor* tempSensor;
        ServoMotor* coverMotor;  
};

#endif
