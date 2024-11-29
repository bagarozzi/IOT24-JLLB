#include <LiquidCrystal_I2C.h>

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
        
        /* Turns off the display. */
        void turnOffDisplay();

        /* Turns on the display. */
        void turnOnDisplay();

    private:

        LiquidCrystal_I2C* lcd;

};