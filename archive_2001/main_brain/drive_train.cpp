/**
 ********************************************************************************************************
 * @file    drive_train.cpp
 * @brief   drive train control methods
 * @details Used to control the drivetrain, turn, etc.
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "drive_train.h"

#include <Servo.h>  // import libraries
#include <PPM.h> // includes the PPM library\


DriveTrain::DriveTrain(int leftPin, int rightPin, int leftInverted, int rightInverted) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  
  // following sets offsets, but it is not fully implemented
  if (leftInverted == 1)
    leftOffset = 180;
  else
    leftOffset = 0;
  if (rightInverted == 1)
    rightOffset = 180;
  else
    rightOffset = 0;
    
}

void DriveTrain::attachMotors() {
  left.attach(_leftPin);
  right.attach(_rightPin);
}

void DriveTrain::moveMotors(int leftVal, int rightVal) {
  // if we're good to move, move the motors
  if(shouldMove) {
    left.write(leftVal);
    right.write(rightVal);
  }
  else
    halt();
}

void DriveTrain::turnLeft(dirTravel dir){
  if (dir == FORWARD) {
    moveMotors(105, 75);
    // left should be slower or backwards
  }
  else { // reverse turn right
    moveMotors(80, 120);
  }
}

void DriveTrain::turnRight(dirTravel dir) {
  if (dir == FORWARD) {
    moveMotors(120, 80);
  }
  else { // reverse turn left
    moveMotors(75, 80);
  }
}

void DriveTrain::sharpTurnLeft (dirTravel dir) {
  if (dir == FORWARD)
    moveMotors(90, 75);
  // reverse direction is broken
//  else 
//    turn(75, 85); // reverse sharp turn RIGHT
}

void DriveTrain::sharpTurnRight(dirTravel dir) {
  if (dir == FORWARD)
    moveMotors(110, 100);
  // reverse direction is broken
//  else
//    turn(105, 120); // reverse sharp turn left

}

void DriveTrain::halt(){
  left.write(leftStop);
  right.write(rightStop);
}

void DriveTrain::forward() {
  moveMotors(110, 80); // 110, 80 is actually forward
}

void DriveTrain::reverse() {
  moveMotors(80, 110); // true reverse!
}

void DriveTrain::turn(int lval, int rval) {
  left.write(lval);
  right.write(rval);
}

void DriveTrain::setTime() {
  startTime = millis();
}

// returns 1 when the turn is done
bool DriveTrain::turn45(bool isRight) {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 550) { // if not gone the time, keep turning
    if (isRight)
      turn(110, 110);
    else
      turn(70, 70);
    return false;
  }
  else {
    halt(); // stop when time has expired
    return true;
  }
}

bool DriveTrain::turn180(bool isRight) {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 1200) { // if not gone the time, keep turning
    if (isRight) {
      turn(110, 110);
    }
    else {
      turn(70, 70);
    }
    return false;
  }
  else { // stop when time expires
    halt();
    return true;
  }
}

bool DriveTrain::turnAround(bool isRight) {
  switch(turnState) {
      // turn until the line sensors only see white
      case TURN_OFF_LINE:
        if (isRight) {
          moveMotors(110, 110);
        }
        else {
          moveMotors(70, 70);
        }
        if ((analogRead(0) < 200) && (analogRead(1) < 200) && (analogRead(2) < 200)) // ALL on white
          turnState = TURN_TILL_LINE;
         break;
       // turn other way until sensors detect black
       case TURN_TILL_LINE:
         if (isRight) {
           moveMotors(110, 110);
         }
         else {
           moveMotors(70, 70);
         }
         if ((analogRead(0) > 200) || (analogRead(1) > 200) || (analogRead(2) > 200)) { // any on black
           turnState = TURN_OFF_LINE;
           return true; // return that the turn is complete
         }
         break;
  }
  return false;
}

bool DriveTrain::backupForTime() {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 650) {
    reverse();
    return false;
  }
  else {
    halt();
    return true;
  }
}

// not debugged
int DriveTrain::backupABit() {
  switch (revState) {
    case INIT_BACKUP:
      setTime();
      revState = BACKUP;
      break;

    case BACKUP:
      bool result = backupForTime();
      if (result) {
        revState = INIT_BACKUP;
        return 1;
      }
      break;
  }
  return 0;
}

bool DriveTrain::forwardForTime() {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 650) {
    moveMotors(110, 80);
    return false;
  }
  else {
    halt();
    return true;
  }
}

// not debugged
int DriveTrain::forwardABit() {
  switch (forwardState) {
    case INIT_FORWARD:
      setTime();
      forwardState = RUN_FORWARD;
      return false;
      break;
    case RUN_FORWARD:
      bool result = forwardForTime();
      if (result) {
        forwardState = INIT_FORWARD;
        return true;
      }
      return false;
      break;
  }
}
