#include "GameInterrupt.h"

#include "constants.h"
#include "GameUtils.h"
#include "Arduino.h"
#include <EnableInterrupt.h>

/* Interrupts functions: */

void wakeUpFunction() {}

void setMainMenuInterrupts() {
  disableAllInterrupts();
  enableInterrupt(POT_PIN, setGameDifficulty, CHANGE); // enableInterrupt(uint8_t pinNumber, void (*userFunction)(void), uint8_t mode)
  enableInterrupt(BTN_PIN[0], handleButton1, RISING);
}

void setMatchInterrupts() {
  disableAllInterrupts();
  enableInterrupt(BTN_PIN[0], handleButton1, RISING);
  enableInterrupt(BTN_PIN[1], handleButton2, RISING);
  enableInterrupt(BTN_PIN[2], handleButton3, RISING);
  enableInterrupt(BTN_PIN[3], handleButton4, RISING);
}

void setSleepInterrupts() {
    for(int i = 0; i < 4; i++) {
        enableInterrupt(BTN_PIN[i], wakeUpFunction, RISING);
    }
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