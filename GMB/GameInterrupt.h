/* Interrupts file */

#pragma once

extern float gameDifficulty;

extern unsigned int preReadPot;

extern unsigned long buttonPressedTime[4];

extern bool BTN_PRESSED[4];

void wakeUpFunction();

void setMainMenuInterrupts();

void setMatchInterrupts();

void setSleepInterrupts();

void disableAllInterrupts();

void resetInput();

void setGameDifficulty();

/* Handles for button's interrupts: */
void handleButtonInterrupt(int buttonIndex);

void handleButton1();

void handleButton2();

void handleButton3();

void handleButton4();