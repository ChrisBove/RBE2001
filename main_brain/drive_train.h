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
    void turnLeft(int dir);
    void turnRight(int dir);
    void sharpTurnLeft(int dir);
    void sharpTurnRight(int dir);
    void halt();
    void forward();
    void reverse();
    void turn(int lval, int rval);
    bool turn45(bool isRight);
    bool turn180(bool isRight);
    void setTime();
    int backupABit();
    bool backupForTime();
    int forwardABit();
    bool turnAround(bool isRight);
    
    enum dirTravel {FORWARD, BACKWARD};
    enum ReverseState {INIT_BACKUP, BACKUP};
    enum ForwardState {INIT_FORWARD, RUN_FORWARD};
    enum turnAroundState {TURN_OFF_LINE, TURN_TILL_LINE};
    turnAroundState turnState = TURN_OFF_LINE;
    ReverseState revState = INIT_BACKUP;
    ForwardState forwardState = INIT_FORWARD;
    
    bool shouldMove = true;
    
  private:
    bool forwardForTime();
  
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
    
    int startTime = 0;
};

#endif
