#include "Printing.h"

void setupLCD() {
    lcd.init();
    lcd.backlight();
}

void printMainMenu() {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Welcome to");
    lcd.setCursor(0, 3);
    lcd.print("\"Give me the binary\"");
}

void printWinningEndgame(int gameScore) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Good job!");
    lcd.setCursor(0, 3);
    lcd.print((String)"Score: " + gameScore);
}

void printLosingEndgame(int gameScore) {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Good job!");
    lcd.setCursor(0, 3);
    lcd.print((String)"Score: " + gameScore);
}
