#include "GameUtils.h"

#include "constants.h"
#include "Printing.h"

#define EASY 0.4
#define MEDIUM 0.5
#define HARD 0.7
#define EXTREME 0.8

#define MATCH_TIME 10000
#define MILLIS_TO_SECOND 1000

#define MAX_NUMBER 15

#define FADE_DELTA 5
#define MIN_FADE_INTENSITY 0
#define MAX_FADE_INTENSITY 255

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
            return EASY;
            break;
        case 2:
            gameTextDifficulty = "Medium";
            return MEDIUM;
            break;
        case 3:
            gameTextDifficulty = "Hard";
            return HARD;
            break;
        case 4:
        case 5:
            gameTextDifficulty = "Extreme";
            return EXTREME;
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
        delay(2000);
        return true;
    }
    return false;
}

long getMatchTime(int score) {
    return MATCH_TIME - ((gameDifficulty * score) * MILLIS_TO_SECOND);
}

int getRandomNumber() {
    return random(MAX_NUMBER + 1);
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
    fadeIntensity = MIN_FADE_INTENSITY;
    fadeDelta = FADE_DELTA;
    analogWrite(RED_PIN, fadeIntensity);
}

void fading() {
    fadeIntensity += fadeDelta;
    if (fadeIntensity >= MAX_FADE_INTENSITY) {
        fadeIntensity = MAX_FADE_INTENSITY;
        fadeDelta = -FADE_DELTA;
    }
    else if (fadeIntensity <= MIN_FADE_INTENSITY) {
        fadeIntensity = MIN_FADE_INTENSITY;
        fadeDelta = FADE_DELTA;
    }
    analogWrite(RED_PIN, fadeIntensity);
    delay(10);
}
