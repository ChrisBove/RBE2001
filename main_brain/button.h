#ifndef button_h
#define button_h

#include "Arduino.h"

class Button {
  public:
    Button(int pin);
    void attachButton();
    
  private:
    int _pin;
};

#endif
