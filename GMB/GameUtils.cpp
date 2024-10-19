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
int fadeIntensity;
int fadeDelta;

float getDifficulty(int potValue) {
    int difficultyLevel = map(potValue, 0, 1023, 1, 5);
    String textDifficulty = "";
    switch (difficultyLevel) {
        case 1:
            gameTextDifficulty = "Easy";
            return EXTREME;
            break;
        case 2:
            gameTextDifficulty = "Medium";
            return HARD;
            break;
        case 3:
            gameTextDifficulty = "Hard";
            return MEDIUM;
            break;
        case 4:
        case 5:
            gameTextDifficulty = "Extreme";
            return EASY;
            break;
        default:
            gameTextDifficulty = "Easy";
            return EASY;
            break;
    }
}

bool checkDifficulty() {
    unsigned int readPot = analogRead(POT_PIN);
    float newGameDifficulty = getDifficulty(readPot);
    if(newGameDifficulty != gameDifficulty) {
        gameDifficulty = newGameDifficulty;
        printDifficulty(gameTextDifficulty);
        delay(1861);
        return true;
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

void initFading() {
    fadeIntensity = 0;
    fadeDelta = 5;
    analogWrite(RED_PIN, fadeIntensity);
}

void fading() {
    fadeIntensity += fadeDelta;
    if (fadeIntensity >= 255) {
        fadeIntensity = 255;
        fadeDelta = -5;
    }
    else if (fadeIntensity <= 0) {
        fadeIntensity = 0;
        fadeDelta = 5;
    }
    analogWrite(RED_PIN, fadeIntensity);
}
