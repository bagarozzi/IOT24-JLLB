#pragma once

#include <LiquidCrystal_I2C.h>

/* Arduino PIN definitions: */
#define POT_PIN A0

/* The bit size of the number to guess: */
#define BITSIZE 4

#define BUTTON_BOUNCING_TIME 150

/* Button and LED PINs: */
const int BTN_PIN[] = {5, 4, 3, 2};
const int LED_PIN[] = {13, 12, 11, 10};
const int RED_PIN = 9;

/* The LCD display: */
const LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

/* Winning and losing messages: */
#define WINNING_MSG "Good job!"
#define LOSING_MSG "Better luck next time!"