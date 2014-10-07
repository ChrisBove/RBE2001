#ifndef gripper_h
#define gripper_h

#include "Arduino.h"

#include <Servo.h>

class Gripper {
  public:
    Gripper(int gripServo, int rackServo);
    void attachMotors();
    void extend();
    void retract();
    void openGrip();
    void closeGrip();
    
  private:
    Servo rack;
    Servo grip;
    int _gripServo;
    int _rackServo;
};

#endif
