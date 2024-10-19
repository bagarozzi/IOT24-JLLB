#include "GameUtils.h"

#include "constants.h"
#include "Printing.h"

#define EASY 0.4
#define MEDIUM 0.5
#define HARD 0.6
#define EXTREME 0.75

extern float gameDifficulty;

unsigned int preReadPot = 100;
String gameTextDifficulty = "";

float getDifficulty(int potValue) {
    int difficultyLevel = map(potValue, 0, 1023, 1, 4);
    String textDifficulty = "";
    switch (difficultyLevel) {
        case 1:
            gameTextDifficulty = "Extreme";
            return EXTREME;
            break;
        case 2:
            gameTextDifficulty = "Hard";
            return HARD;
            break;
        case 3:
            gameTextDifficulty = "Medium";
            return MEDIUM;
            break;
        case 4:
            gameTextDifficulty = "Easy";
            return EASY;
            break;
    }
}

bool checkDifficulty() {
  unsigned int readPot = analogRead(POT_PIN);
  if(readPot >= (preReadPot + 2) || readPot <= (preReadPot - 2)) {
    preReadPot = readPot;
    float newGameDifficulty = getDifficulty(readPot);
    if(newGameDifficulty == gameDifficulty) {
      gameDifficulty = newGameDifficulty;
      printDifficulty(gameTextDifficulty);
      delay(1861);
      return true;
    }
  }
  return false;
}

long getMatchTime(float difficulty, int score) {
    return 10000 - ((difficulty * score) * 1000);
}

int getRandomNumber() {
    return random(0, 16);
}

void writeBinaryNumber(int number, bool* binaryNumber) {
    for (int i = 0; i < BITSIZE; i++) {
        binaryNumber[BITSIZE - 1 - i] = (number & (1 << i)) ? true : false;
    }
}

bool checkGuess(bool* guessedNumber, bool* trueNumber) {
    for (int i = 0; i < BITSIZE; i++) {
        if (guessedNumber[i] != trueNumber[i]) {
        return false;
        }
    }
    return true;
}