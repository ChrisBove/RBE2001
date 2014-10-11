#ifndef arm_h
#define arm_h

#include <Servo.h>
#include "button.h"

class Arm {
  
  public:
    Arm(int motorPin, int potPin);
    void setupArm();
    void setPosition(int setPoint, Button& frontLimit);
    bool isInPosition();
    
    bool goUp(Button& frontLimit);
    bool goDown(Button& frontLimit);
    
    int goodness = 0;
  private:
    void rotateDown (float output, Button& frontLimit);
    void rotateUp (float output, Button& frontLimit);
    
    void sendOutput(float output, Button& frontLimit);
    float scaleOutput (float output);
    
    int _motorPin;
    int _potPin;
    
     // constants for PID, casually tuned for system
     const float Kp = 0.0001;
     const float Ki = 0.0000005; 
     const float Kd = 0.001;
     
     // ****** GLOBALS ********
     float lastError = 0; // stored from previous value.
     float errorSum = 0;  // sum of errors
     int count = 0;  // used to reset sum
};

#endif
