#ifndef gripper_h
#define gripper_h

#include "Arduino.h"

#include <Servo.h>

class Gripper {
  public:
  enum gripperState {WAIT_FOR_INSTRUCT, INIT_CLOSE_GRIP, CLOSE_GRIP, INIT_RETRACT, RETRACT, INIT_EXTEND, EXTEND, INIT_EXTENDLIM, EXTENDLIM, INIT_OPEN_GRIP, OPEN_GRIP};
  Gripper(gripperState state);
    
    gripperState grippyStateClose = INIT_CLOSE_GRIP;
    gripperState grippyStateRetract = INIT_RETRACT;
    gripperState grippyStateExtend = INIT_EXTEND;
    gripperState grippyStateExtendLim = INIT_EXTENDLIM;
    gripperState grippyStateOpen = INIT_OPEN_GRIP;
    
    Gripper(int gripServo, int rackServo);
    void attachMotors();
    bool closeTheGrip();
    bool retractTheGrip();
    bool extendTheGrip();
    bool extendLimTheGrip();
    bool openTheGrip();
    bool extend();
    bool extendLim();
    bool retract();
    bool openGrip();
    bool closeGrip();
    void setReactTime();
    
  private:
    Servo rack;
    Servo grip;
    int _gripServo;
    int _rackServo;
    int reactTime = 0;
};

#endif
