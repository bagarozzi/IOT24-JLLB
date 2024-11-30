#ifndef __DISPLAY_SERVICE__
#define __DISPLAY_SERVICE__
#include <LiquidCrystal_I2C.h>
#include "Button.h"
#include "config.h"
class DisplayService { 

    public:

        DisplayService();
        void init();
        
        /* Displays the initial "Press OPEN to enter waste" message. */
        void displayInitialMessage();

        /* Displays the "Press CLOSE when done" message. Toghether with the time 
        remaining */
        void displayClosingMessage(int timeLeft);

        /* Displays the "Waste received" message. */
        void displayFinalDisposingMessage();

        /* Displays the "Container full" message. */
        void displayContainerFullMessage();

        /* Displays the "Temperature too high message" */
        void displayHighTemperatureMessage();
        
        /* Turns off the display. */
        void turnOffDisplay();

        /* Turns on the display. */
        void turnOnDisplay();

        /* synchronises the buttons (open e close)*/
        void synchroniseButton();

        /* checks if the open button in pressed*/
        bool isOpenButtonPressed();

        /* checks if the close button in pressed*/
        bool isCloseButtonPressed();

    private:

        LiquidCrystal_I2C* lcd;
        Button* openButton;
        Button* closeButton;

};

#endif
