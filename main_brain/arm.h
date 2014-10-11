#ifndef arm_h
#define arm_h

#include <Servo.h>
#include "button.h"

class Arm {
  
  public:
    Arm(int motorPin, int potPin, Button& frontLimit, Button& backLimit);
    void setupArm();
    
  private:
    int _motorPin;
    int _potPin;
};

#endif
