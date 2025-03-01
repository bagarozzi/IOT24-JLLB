#include "model/DisplayService.h"
#include "DisplayService.h"
#include "ButtonImpl.h"

DisplayService::DisplayService() {
    lcd = new LiquidCrystal_I2C(0x27, 20, 4);
    openButton = new ButtonImpl(OPEN_BUTTON_PIN);
    closeButton = new ButtonImpl(CLOSE_BUTTON_PIN);
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

void DisplayService::displayHighTemperatureMessage() {
    lcd->clear();
    lcd->setCursor(2, 0);
    lcd->print("Temperature too");
    lcd->setCursor(7, 1);
    lcd->print("high");
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

void DisplayService::synchroniseButton() {
    openButton->sync();
    closeButton->sync();
}

bool DisplayService::isOpenButtonPressed() {
    return openButton->isPressed();
}

bool DisplayService::isCloseButtonPressed() {
    return closeButton->isPressed();
}
