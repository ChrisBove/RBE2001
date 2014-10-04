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
    void attachMotors();
    
    void moveMotors(int leftVal, int rightVal);
    void turnLeft();
    void turnRight();
    void halt();
    void forward();
    void turn(int lval, int rval);
    
    
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
    
    int leftStop = 90;
    int rightStop = 90;
    
    int leftOffset = 0;
    int rightOffset = 0;
    
};

#endif
