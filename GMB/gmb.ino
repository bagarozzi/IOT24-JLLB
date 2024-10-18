#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#include <TimerOne.h>
#include <avr/sleep.h>


#include "GameUtils.h"
#include "Printing.h"

/* Wiring: SDA => A4, SCL => A5 */
/* I2C address of the LCD: 0x27 */
/* Number of columns: 20 rows: 4 */
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

/* Arduino PIN definitions: */
#define POT_PIN A0
#define SDA A4
#define SCL A5

#define BITSIZE 4

#define BUTTON_BOUNCING_TIME 100

/* Button and LED PINs: */
int BTN_PIN[] = {5, 4, 3, 2};
int LED_PIN[] = {13, 12, 11, 10};
const int RED_PIN = 9;
bool BTN_PRESSED[] = {false, false, false, false};

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
  }
}

void matchInit() {
  currentNumber = getRandomNumber();
  for (int i = 0; i < BITSIZE; i++) {
    currentBinaryNumber[BITSIZE - 1 - i] = (currentNumber & (1 << i)) ? true : false;
  }
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
  bool corrispondono = true;
  for (int i = 0; i < BITSIZE; i++) {
    if (currentBinaryNumber[i] != BTN_PRESSED[i]) {
      corrispondono = false;
      break;
    }
  }
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
  for(int i = 0; i < 4; i++) {
    enableInterrupt(BTN_PIN[i], wakeUpFunction, RISING);
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  shutdownTime = 0;
  //previousLoop = millis();
  gamePhase = mainMenuState;
}

void wakeUpFunction() {}

/* Interrupts functions: */

void setMainMenuInterrupts() {
  disableAllInterrupts();
  enableInterrupt(POT_PIN, setGameDifficulty, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt(BTN_PIN[0], handleButton1, RISING);
}

void setMatchInterrupts() {
  disableAllInterrupts();
  enableInterrupt(POT_PIN, setGameDifficulty, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt(BTN_PIN[0], handleButton1, RISING);
  enableInterrupt(BTN_PIN[1], handleButton2, RISING);
  enableInterrupt(BTN_PIN[2], handleButton3, RISING);
  enableInterrupt(BTN_PIN[3], handleButton4, RISING);
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
    digitalWrite(LED_PIN[i], LOW);
  }
}

void setGameDifficulty() {
  unsigned int readPot = analogRead(POT_PIN);
  if(readPot >= (preReadPot + 2) || readPot <= (preReadPot - 2))
    gameDifficulty = getDifficulty(readPot);
}

/* Handles for button's interrupts: */
void handleButtonInterrupt(int buttonIndex) {
  unsigned long currentTime = millis();

  if (currentTime - buttonPressedTime[buttonIndex] > BUTTON_BOUNCING_TIME) {
    buttonPressedTime[buttonIndex] = currentTime;
    BTN_PRESSED[buttonIndex] = !BTN_PRESSED[buttonIndex];
    if (BTN_PRESSED[buttonIndex]) {
      digitalWrite(LED_PIN[buttonIndex], HIGH);
    }
    else {
      digitalWrite(LED_PIN[buttonIndex], LOW);
    }  
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