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

int BTN_PIN[] = {5, 4, 3, 2};
int LED_PIN[] = {13, 12, 11, 10};
const int RED_PIN = 9;
bool BTN_PRESSED[] = {false, false, false, false};

/* Global variables: */
float gameDifficulty = 0.8 // Defaults to "easy"
int gameScore = 0;
long matchDuraion = 10000;

void (*gamePhase)(void) = mainMenuState();
long elapsedTime = millis();
long shutdownTime = 0;

#define BITSIZE 4
int currentNumber = 0;
bool currentBinaryNumber[BITSIZE] = {false, false, false, false};


void setup() {
  for(int i = 0; i < 4; i++) {
    pinMode(LED_PIN[i], OUTPUT);
    pinMode(BTN_PIN[i], INPUT);
  }
  pinMode(RED_PIN, OUTPUT);
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
    gamePhase = matchInit;
    BTN_PRESSED[0] = false;
  }
}

void matchInit() {
  currentNumber = random(0, 16);
  for (int i = 0; i < BITSIZE; i++) {
    currentBinaryNumber[BITSIZE - 1 - i] = (currentNumber & (1 << i)) ? true : false;
  }
  matchDuraion = getMatchTime(gameDifficulty, gameScore);
  resetInput();
  gamePhase = matchState;
}

void matchState() {
  matchDuration -= elapsedTime;
  if (matchDuraion <= 0) {
   gamePhase = endGameState; 
  }
}

void endGameState() {
  bool corrispondono = true;
  for (int i = 0; i < BITSIZE; i++) {
    if (currentBinaryNumber[i] != BTN_PRESSED[i]) {
      corrispondono = false;
      break;
    }
  }
  if (corrispondono) {
    gameScore++;
    lcd.setCursor(5, 1);
    lcd.print("Good job!");
    lcd.setCursor(0, 3);
    // VEDERE SE FUNZIONA
    lcd.print("Score: " + score);
    gamePhase = matchInit;
  }
  else {
    digitalWrite(RED_PIN, HIGH);
    delay(1000);
    digitalWrite(RED_PIN, LOW);
    lcd.setCursor(5, 1);
    lcd.print("Game over!");
    lcd.setCursor(0, 3);
    // VEDERE SE FUNZIONA
    lcd.print("Final score: " + score);
    gameScore = 0;
    delay(10000);
    gamePhase = mainMenuState;
  }
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

void wakeUpFunction() {}

/* Interrupts functions: */

void setMainMenuInterrupts() {
  disableAllInterrupts();
  enableInterrupt(POT_PIN, setGameDifficulty, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt(BTN_PIN[0], handleButton1, RISING);
}

void disableAllInterrupts() {
  for (int i = 2; i < 13; i++) {
    disableInterrupt(i);
  }
  disableInterrupt(A0);
}

void resetInput() {
  for(int i = 0; i < 4; i++) {
    BTN_PRESSED[i] = false;
    digitalWrite(BTN_PIN[i], LOW);
  }
}

void setGameDifficulty() {
  gameDifficulty = getDifficulty(analogRead(potPin));
}

/* Handles for button's interrupts: */
void handleButtonInterrupt(int buttonIndex) {
  BTN_PRESSED[buttonIndex] = !BTN_PRESSED[buttonIndex];
  if (BTN_PRESSED[buttonIndex]) {
    digitalWrite(BTN_PIN[buttonIndex], HIGH);
  }
  else {
    digitalWrite(BTN_PIN[buttonIndex], LOW);
  }
}

void handleButton1() {
  handleButtonInterrupt(0);
}

void handleButton2() {
  handleButtonInterrupt(1);
}

void handleButton3() {
  handleButtonInterrupt(2);
}

void handleButton4() {
  handleButtonInterrupt(3);
}