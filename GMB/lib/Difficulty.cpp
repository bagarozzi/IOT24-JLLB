#include "Difficulty.h"

float getDifficulty(int potValue) {
    int difficultyLevel = map(potValue, 0, 1023, 1, 4);
    int difficultyFactor = 0;
    String textDifficulty = "";
    switch (difficultyLevel) {
        case 1:
            difficultyFactor = 1.5;
            textDifficulty = "Extreme";
            break;
        case 2:
            difficultyFactor = 1.2;
            textDifficulty = "Hard";
            break;
        case 3:
            difficultyFactor = 1.0;
            textDifficulty = "Medium";
            break;
        case 4:
            difficultyFactor = 0.8;
            textDifficulty = "Easy";
            break;
    }
    /* CHIAMARE FUNZIONE STAMPA NOSTRA LIBRERIA PASSANDO TEXT DIFFICULTY */
    return difficultyFactor;
}
