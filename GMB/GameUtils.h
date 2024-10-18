/*
 * A file for all the function related to the game. 
*/

#pragma once

float getDifficulty(int potValue);

long getMatchTime(float difficulty, int score);

int getRandomNumber();

void writeBinaryNumber(int number, bool* binaryNumber);

bool checkGuess(bool* guessedNumber, bool* trueNumber);