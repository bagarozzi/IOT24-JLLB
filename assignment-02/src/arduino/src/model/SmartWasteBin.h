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


    private:

    enum {
        IDLE,
        BIN_OPEN
        // and so on
    } state; 

    Led* Led1;
    Led* Led2;

    Sonar* Sonar;
    Pir* Pir;
    TempSensor* TempSensor;
    ServoMotor* CoverMotor;  
};

#endif
