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

#define B1_PIN 2
#define B2_PIN 3
#define B3_PIN 4
#define B4_PIN 5

#define L1_PIN 10
#define L2_PIN 11
#define L3_PIN 12
#define L4_PIN 13
#define RED_PIN 9

/* Global variables: */
float gameDifficulty = 0.8 // Defaults to "easy"


void setup() {
  lcd.init();
  lcd.backlight();
  Timer1.initialize(10000);
  Timer1.attachInterrupt(sleepGame);
}

void loop() {

}

void potInterrupt() {
  gameDifficulty = setDifficulty(analogRead(potPin));
}

void sleepState() {
  // vai a letto
}

void mainMenuState() {
  /* Set the cursor on the third column and first row. */
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 3);
  lcd.print("\"Give me the binary\"");
  Timer1.start()
  /* Se il gioco è iniziato allora: 
  Timer1.stop(), 
  se si torna alla schermata iniziale allora Timer1.restart()*/
}

void gameState() {

}

void endGameState() {

}

void setMainMenuInterrupts() {
  disableAllInterrupts();
  enableInterrupt(potPin, potInterrupt, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt()
}

void disableAllInterrupts() {
  for (int i = 2; i < 13; i++) {
    disableInterrupt(i);
  }
  disableInterrupt(A0);
}

/*
void difficulty() {
  int newDifficulty = 1; //map(analogRead(potPin), 0, 1023, 1, 4);
  switch (difficultyLevel) {

  }
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Difficulty");
  lcd.setCursor(10, 3);
  lcd.print(newDifficulty);
}
*/