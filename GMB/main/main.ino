#include <LiquidCrystal_I2C.h>

/* Wiring: SDA => A4, SCL => A5 */
/* I2C address of the LCD: 0x27 */
/* Number of columns: 20 rows: 4 */
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

void setup() {
  lcd.init();
  lcd.backlight();
}
void loop() {
  /* Set the cursor on the third column and first row. */
  lcd.setCursor(5, 1);
  lcd.print("Welcome to");
  lcd.setCursor(0, 3);
  lcd.print("\"Give me the binary\"");
  delay(1000);
  difficulty();
  delay(1000);
  lcd.clear();

}

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