#include "GameUtils.h"
#include "Arduino.h"
#include "constants.h"

float getDifficulty(int potValue) {
    int difficultyLevel = map(potValue, 0, 1023, 1, 4);
    int difficultyFactor = 0;
    String textDifficulty = "";
    switch (difficultyLevel) {
        case 1:
            difficultyFactor = 0.75;
            textDifficulty = "Extreme";
            break;
        case 2:
            difficultyFactor = 0.6;
            textDifficulty = "Hard";
            break;
        case 3:
            difficultyFactor = 0.5;
            textDifficulty = "Medium";
            break;
        case 4:
            difficultyFactor = 0.4;
            textDifficulty = "Easy";
            break;
    }
    /* CHIAMARE FUNZIONE STAMPA NOSTRA LIBRERIA PASSANDO TEXT DIFFICULTY */
    return difficultyFactor;
}

long getMatchTime(float difficulty, int score) {
    return 10000 - ((difficulty * score) * 1000);
}

int getRandomNumber() {
    return random(0, 16);
}

void writeBinaryNumber(int number, bool* binaryNumber) {
    for (int i = 0; i < 4; i++) {
        binaryNumber[4 - 1 - i] = (number & (1 << i)) ? true : false;
    }
}