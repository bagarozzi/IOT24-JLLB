#include <avr/sleep.h>

#include "GameUtils.h"
#include "Printing.h"
#include "constants.h"
#include "GameInterrupt.h"

/* Match variables: */
float gameDifficulty = 0.4; // Defaults to "easy"
int gameScore = 0;
long matchDuration = 10000;
int currentNumber = 0;
bool currentBinaryNumber[BITSIZE] = {false, false, false, false};

/* Architecture variables: */
void (*gamePhase)(void);
unsigned long elapsedTime = millis();
unsigned long previousLoop = 0;

/* Interrupt variables: */
unsigned long shutdownTime = 0;

/* Array for pressed buttons: */
bool BTN_PRESSED[] = {false, false, false, false};

void setup() {
  gamePhase = mainMenuState;
  for(int i = 0; i < 4; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    pinMode(BTN_PIN[i], INPUT);
  }
  pinMode(RED_PIN, OUTPUT);
  setupLCD();
  elapsedTime = millis();
}

void loop() {
  elapsedTime = millis() - previousLoop;
  previousLoop = millis();
  gamePhase();
}

/*
 * The main menu of the game. 
 */
void mainMenuState() {
  setMainMenuInterrupts();
  printMainMenu();
  gamePhase = waitState;
}

/** 
 * The idle state of the main menu before sleeping or starting the match:
 * 
 */
void waitState() {
  shutdownTime += elapsedTime;
  if(shutdownTime >= 10000) {
    gamePhase = sleepState;
  }
  else if(BTN_PRESSED[0]) {
    gamePhase = matchInit;
    BTN_PRESSED[0] = false;
    digitalWrite(LED_PIN[0], LOW);
  }
}

/** 
 * Initialise the match: resets variables, sets the correct interrupts.
 */
void matchInit() {
  currentNumber = getRandomNumber();
  writeBinaryNumber(currentNumber, currentBinaryNumber);
  matchDuration = getMatchTime(gameDifficulty, gameScore);
  setMatchInterrupts();
  printMatch(currentNumber);
  gamePhase = matchState;
}

/** 
 * The match state in which the user plays.
 */
void matchState() {
  matchDuration -= elapsedTime;
  if (matchDuration <= 0) {
   gamePhase = endGameState; 
  }
}

/** 
 * The endgame state, the user is displayed the result. The game later continues automatically.
 */
void endGameState() {
  bool corrispondono = checkGuess(currentBinaryNumber, BTN_PRESSED);
  if (corrispondono) {
    gameScore++;
    printEndgame(gameScore, WINNING_MSG);
    delay(1861);
    gamePhase = matchInit;
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    delay(1000);
    digitalWrite(RED_PIN, LOW);
    printEndgame(gameScore, LOSING_MSG);
    gameScore = 0;
    delay(10000);
    gamePhase = mainMenuState;
  }
  resetInput();
}

/*
 * The state in which the Arduino sleeps until is woken up. 
 */
void sleepState() {
  disableAllInterrupts();
  setSleepInterrupts();
  clearDisplay();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  shutdownTime = 0;
  gamePhase = mainMenuState;
}