/* Interrupts file */

#pragma once

extern float gameDifficulty;

extern unsigned int preReadPot;

extern unsigned long buttonPressedTime[4];

extern bool BTN_PRESSED[4];

/** 
 * Dummy function to be called upon waking up
 * from the deep sleep.
 */
void wakeUpFunction();

/**
 * Sets the interrupts for the Main Menu phase of the game.
 */
void setMainMenuInterrupts();

/**
 * Sets the interrupts for the main Match phase of the game.
 */
void setMatchInterrupts();

/**
 * Sets the interrupts for the Sleep phase of the game.
 */
void setSleepInterrupts();

/**
 * Disables all interrupts set on the board.
 */
void disableAllInterrupts();

/** 
 * Resets the input vector.
 */
void resetInput();

/** 
 * Sets the game difficulty after the potentiometer has been twisted.
 */
void setGameDifficulty();

/**
 * Follows a series of function to manage the interrupts of the four 
 * buttons.
 */

/** 
 * This is a generic function for handling an interrupt in the "buttonIndex" 
 * pin of the button's array.
 */
void handleButtonInterrupt(int buttonIndex);

void handleButton1();

void handleButton2();

void handleButton3();

void handleButton4();