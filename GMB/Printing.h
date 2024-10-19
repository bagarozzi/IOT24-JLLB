#include "constants.h"

/** 
 * Sets up the LCD display.
 */
void setupLCD();

/** 
 * Prints the welcome message to the display.
 */
void printMainMenu();

/**
 * Prints the number to guess to the display.
 */
void printMatch(int currentNumber);

/** 
 * Prints the endgame message with the current score.
 */
void printEndgame(int gameScore, String message);


void printDifficulty(int difficulty);