#pragma once

#include "Arduino.h"
#include "constants.h"

/** 
 * Returns the difficulty of the game given the value of the
 * potentiometer. 
 */
float getDifficulty(int potValue);

bool checkDifficulty();

void setDifficulty();

/** 
 * Returns the match time given the difficulty of the match and the current score.
 */
long getMatchTime(float difficulty, int score);

/** 
 * Returns a random number between 0 and 15 (inclusive).
 */
int getRandomNumber();

/** 
 * Converts "number" into a binary number and copies it into "binaryNumber", 
 * the most significant bit in position "0".
 */
void writeBinaryNumber(int number, bool* binaryNumber);

/** 
 * Returns wether the binary number (represented as arrays of boolean)
 * are equal or not.
 */
bool checkGuess(bool* guessedNumber, bool* trueNumber);