/*
  
*/

#ifndef drive_train_h
#define drive_train_h

#include "Arduino.h"

#include <Servo.h>  // import libraries


class DriveTrain
{
  public:
    DriveTrain(int leftPin, int rightPin, int leftInverted, int rightInverted);
    void MoveMotors(int leftVal, int rightVal);
    
    void AttachMotors();
    
  private:
    
  
    // VARIABLES
    Servo left;       // Declare drive motors
    Servo right;
    int _leftPin;
    int _rightPin;
    
    const int debug = true;  // enables/disables debug prints
    
    //inititialize the motor drive constants to "stopped"
    int _leftDrive = 90;  
    int _rightDrive = 90;
    
    int leftOffset = 0;
    int rightOffset = 0;
    
};

#endif
