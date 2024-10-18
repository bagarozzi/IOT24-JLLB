/*
 * A library for calculating and showing the game's difficulty set by a potentiometer.
*/

float getDifficulty(int potValue);

long getMatchTime(float difficulty, int score);

int getRandomNumber();

void writeBinaryNumber(int number, bool* binaryNumber);

bool checkGuess(bool* guessedNumber, bool* trueNumber);