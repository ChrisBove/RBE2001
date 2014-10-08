#ifndef gripper_h
#define gripper_h

#include "Arduino.h"

#include <Servo.h>

class Gripper {
  public:
  enum gripperState {WAIT_FOR_INSTRUCT, INIT_CGRIP, CGRIP, INIT_RETRACT1, RETRACT, INIT_EXTEND, EXTEND, INIT_OGRIP1, OGRIP1, INIT_CGRIP2, CGRIP2, INIT_RETRACT2};
  Gripper(gripperState state);
    
    gripperState thoughtState = INIT_CGRIP;
    
    Gripper(int gripServo, int rackServo);
    void attachMotors();
    bool extend();
    bool retract();
    bool openGrip();
//    bool closeGrip();
    void setReactTime();
    
  private:
  bool closeGrip();
    Servo rack;
    Servo grip;
    int _gripServo;
    int _rackServo;
    int reactTime = 0;
};

#endif
