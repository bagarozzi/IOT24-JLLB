#include "model/DisplayService.h"
#include "DisplayService.h"

DisplayService::DisplayService() {
    lcd = new LiquidCrystal_I2C(0x27, 20, 4);
}

void DisplayService::init() {
    lcd->init();
    lcd->backlight();
    lcd->noDisplay();
}

void DisplayService::displayInitialMessage() {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Press OPEN to");
    lcd->setCursor(0, 1);
    lcd->print("enter waste");
    lcd->display();
}

void DisplayService::displayClosingMessage(int timeLeft) {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Press CLOSE when");
    lcd->setCursor(0, 1);
    lcd->print("done");
    lcd->setCursor(0, 2);
    lcd->print("Time left: ");
    lcd->print(timeLeft);
    lcd->display();
}

void DisplayService::displayFinalDisposingMessage() {
    lcd->clear();
    lcd->setCursor(2, 0);
    lcd->print("Waste received");
    lcd->display();
}

void DisplayService::displayContainerFullMessage() {
    lcd->clear();
    lcd->setCursor(2, 0);
    lcd->print("Container full");
    lcd->display();
}

void DisplayService::turnOffDisplay() {
    lcd->noDisplay();
    lcd->noBacklight();
}

void DisplayService::turnOnDisplay() {
    lcd->display();
    lcd->backlight();
}
