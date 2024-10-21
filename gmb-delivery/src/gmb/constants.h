#pragma once

#include <LiquidCrystal_I2C.h>

/* Arduino PIN definitions: */
#define POT_PIN A0

/* The bit size of the number to guess: */
#define BITSIZE 4

#define BUTTON_BOUNCING_TIME 150

/* Game's difficulty values:  */
#define EASY 0.4
#define MEDIUM 0.5
#define HARD 0.7
#define EXTREME 0.8

/* Time values: */
#define MATCH_TIME 10000
#define MILLIS_TO_SECOND 1000

/* Superior bound of guessable numbers: */
#define MAX_NUMBER 15

/* Red LED fade values: */
#define FADE_DELTA 5
#define MIN_FADE_INTENSITY 0
#define MAX_FADE_INTENSITY 255

/* Button and LED PINs: */
const int BTN_PIN[] = {5, 4, 3, 2};
const int LED_PIN[] = {13, 12, 11, 10};
const int RED_PIN = 9;

/* The LCD display: */
const LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

/* Winning and losing messages: */
#define WINNING_MSG "Good Job!"
#define LOSING_MSG "Game Over!"