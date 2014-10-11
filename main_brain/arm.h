#ifndef arm_h
#define arm_h

#include <Servo.h>

class Arm {
  
  public:
    Arm(int motorPin, int potPin);
    void setupArm();
    
  private:
    int _motorPin;
    int _potPin;
};

#endif
