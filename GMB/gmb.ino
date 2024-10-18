#include <LiquidCrystal_I2C.h>
#include <avr/sleep.h>

#include "GameUtils.h"
#include "Printing.h"
#include "constants.h"
#include "GameInterrupt.h"
/* Wiring: SDA => A4, SCL => A5 */
/* I2C address of the LCD: 0x27 */
/* Number of columns: 20 rows: 4 */
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

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
  lcd.init();
  lcd.backlight();
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
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 3);
  lcd.print("\"Give me the binary\"");
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
  lcd.clear();
  lcd.setCursor(9, 2);
  lcd.print(currentNumber);
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
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Good job!");
    lcd.setCursor(0, 3);
    // VEDERE SE FUNZIONA
    lcd.print((String)"Score: " + gameScore);
    delay(1861);
    gamePhase = matchInit;
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    delay(1000);
    digitalWrite(RED_PIN, LOW);
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Game over!");
    lcd.setCursor(0, 3);
    // VEDERE SE FUNZIONA
    lcd.print((String)"Final score: " + gameScore);
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