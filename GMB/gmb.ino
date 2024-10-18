#include <avr/sleep.h>

#include "GameUtils.h"
#include "Printing.h"
#include "constants.h"
#include "GameInterrupt.h"
/* Wiring: SDA => A4, SCL => A5 */
/* I2C address of the LCD: 0x27 */
/* Number of columns: 20 rows: 4 */

/* Match variables: */
float gameDifficulty = 0.8; // Defaults to "easy"
int gameScore = 0;
long matchDuration = 10000;
void (*gamePhase)(void);
unsigned long elapsedTime = millis();
unsigned long previousLoop = 0;
unsigned long shutdownTime = 0;
unsigned int preReadPot = 0;
unsigned long buttonPressedTime[4];
int currentNumber = 0;
bool currentBinaryNumber[BITSIZE] = {false, false, false, false};
bool BTN_PRESSED[] = {false, false, false, false};

void setup() {
  Serial.begin(9600);
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

void waitState() {
  Serial.println(shutdownTime);
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

void matchInit() {
  currentNumber = getRandomNumber();
  writeBinaryNumber(currentNumber, currentBinaryNumber);
  matchDuration = getMatchTime(gameDifficulty, gameScore);
  setMatchInterrupts();
  printMatch(currentNumber);
  gamePhase = matchState;
}

void matchState() {
  matchDuration -= elapsedTime;
  if (matchDuration <= 0) {
   gamePhase = endGameState; 
  }
}

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
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  shutdownTime = 0;
  //previousLoop = millis();
  gamePhase = mainMenuState;
}