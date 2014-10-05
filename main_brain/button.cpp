#include "button.h"

Button::Button(int pin) {
  _pin = pin;
}

void Button::setupButton() {
  pinMode(_pin, INPUT_PULLUP);
}

bool Button::isBumped() {
  return !digitalRead(_pin);
}
