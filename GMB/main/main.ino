#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#include <TimerOne.h>

#include <Difficulty.h>
#include <Printing.h>

/* Wiring: SDA => A4, SCL => A5 */
/* I2C address of the LCD: 0x27 */
/* Number of columns: 20 rows: 4 */
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

/* Arduino PIN definitions: */
#define POT_PIN A0

#define SDA A4
#define SCL A5

int BTN_PIN[] = {2, 3, 4, 5};
int LED_PIN[] = {10, 11, 12, 13, 9};

/* Global variables: */
float gameDifficulty = 0.8 // Defaults to "easy"

void (*gamePhase)(void) = mainMenuState();
long elapsedTime = millis();
long shutdownTime = 0;

bool BTN_PRESSED[] = {false, false, false, false};
#define BITSIZE 4
int currentNumber = 0;
bool currentBinaryNumber[BITSIZE] = {false, false, false, false};

void setup() {
  lcd.init();
  lcd.backlight();
  Timer1.initialize(10000);
  Timer1.attachInterrupt(sleepGame);
  elapsedTime = millis();
}

void loop() {
  elapsedTime = millis() - elapsedTime;
  gamePhase();
}

/*
 * The main menu of the game. 
 */
void mainMenuState() {
  setMainMenuInterrupts();
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 3);
  lcd.print("\"Give me the binary\"");
  gamePhase = waitState;
}

void waitState() {
  shutdownTime += elapsedTime;
  if(shutdownTime >= 10000) {
    gamePhase = sleepState;
  }
  else if(BTN_PRESSED[0]) {
    gamePhase = matchState;
    BTN_PRESSED[0] = false;
  }
}

void matchInit() {
  currentNumber = random(0, 16);
  for (int i = 0; i < BITSIZE; i++) {
    currentBinaryNumber[BITSIZE - 1 - i] = (currentNumber & (1 << i)) ? true : false;
  }
}

void matchState() {
  matchInit();

  gamePhase = endGameState;
}

void endGameState() {
  // Confronta i vettori
  bool corrispondono = true;
  for (int i = 0; i < BITSIZE; i++) {
    if (currentBinaryNumber[i] != BTN_PRESSED[i]) {
      corrispondono = false;
      break;
    }
  }
  gamePhase = matchState;
  
  gamePhase = mainMenuState;
}

/*
 * The state in which the Arduino sleeps until is woken up. 
 */
void sleepState() {
  disableAllInterrupts();
  for(int i = 0; i < 3; i++) {
    enableInterrupt(BTN_PIN[i], wakeUpFunction, RISING);
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  gamePhase = mainMenuState;
}

void startGameInterrupt() {
  BTN_PRESSED[0] = true;
}

void wakeUpFunction() {}

/* Interrupts functions: */

void setMainMenuInterrupts() {
  disableAllInterrupts();
  enableInterrupt(POT_PIN, setGameDifficulty, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt(BTN_PIN[0], startGameInterrupt, RISING);
}

void disableAllInterrupts() {
  for (int i = 2; i < 13; i++) {
    disableInterrupt(i);
  }
  disableInterrupt(A0);
}

void setGameDifficulty() {
  gameDifficulty = getDifficulty(analogRead(potPin));
}