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

void printMatch(int currentNumber) {
    lcd.clear();
    lcd.setCursor(9, 2);
    lcd.print(currentNumber);
}

void printEndgame(int gameScore, String message) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(0, 3);
    lcd.print((String)"Score: " + gameScore);
}

void printDifficulty(String difficulty) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Game difficulty: ");
    lcd.setCursor(8, 2);
    lcd.print(difficulty);
}

void clearDisplay() {
    lcd.clear();
}