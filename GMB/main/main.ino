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
#define potPin A0

/* Global variables: */
float gameDifficulty = 0.8 // Defaults to "easy"


void setup() {
  lcd.init();
  lcd.backlight();
  enableInterrupt(potPin, potInterrupt, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
}

void loop() {
  /* Set the cursor on the third column and first row. */
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 3);
  lcd.print("\"Give me the binary\"");
  /* TIMER 10s che aspetta e si va a letto */
  
  lcd.clear();
}

void potInterrupt() {
  gameDifficulty = setDifficulty(analogRead(potPin));
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