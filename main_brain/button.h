#ifndef button_h
#define button_h

#include "Arduino.h"

class Button {
  public:
    Button(int pin);
    void setupButton();
    bool isBumped();
    
  private:
    int _pin;
};

#endif
