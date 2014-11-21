/**
 ********************************************************************************************************
 * @file    button.cpp
 * @brief   button control methods
 * @details Used to read buttons
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/
#include "button.h"

Button::Button(int pin) {
  _pin = pin;
}

void Button::setupButton() {
  pinMode(_pin, INPUT_PULLUP); // configure as pullup input
}

bool Button::isBumped() {
  return !digitalRead(_pin); // invert logic, and return it
}
