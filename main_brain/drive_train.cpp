#include "Arduino.h"
#include "drive_train.h"

#include <Servo.h>  // import libraries
#include <PPM.h> // includes the PPM library\


DriveTrain::DriveTrain(int leftPin, int rightPin, int leftInverted, int rightInverted) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  
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
  _leftDrive = leftVal;
  _rightDrive = rightVal;
  left.write(leftOffset - _leftDrive);
  right.write(_rightDrive);
}

void DriveTrain::turnLeft(int dir){
  if (dir == FORWARD) {
    left.write(105); // should be slower or backwards
    right.write(70);
  }
  else { // reverse turn right
    left.write(80); // should be slower or backwards
    right.write(120);
  }
}

void DriveTrain::turnRight(int dir) {
  if (dir == FORWARD) {
    left.write(110);
    right.write(80);
  }
  else { // reverse turn left
    left.write(75);
    right.write(105);
  }
}

void DriveTrain::sharpTurnLeft (int dir) {
  if (dir == FORWARD)
    turn(80, 75);
//  else 
//    turn(75, 85); // reverse sharp turn RIGHT
}

void DriveTrain::sharpTurnRight(int dir) {
  if (dir == FORWARD)
    turn(110, 100);
//  else
//    turn(105, 120); // reverse sharp turn left

}

void DriveTrain::halt(){
  left.write(leftStop);
  right.write(rightStop);
}

void DriveTrain::forward() {
  left.write(110); // 110, 80 is actually forward
  right.write(70); 
}

void DriveTrain::reverse() {
  left.write(78);
  right.write(110); 
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
  if (timeLapse <= 400) {
    if (isRight)
      turn(110, 110);
    else
      turn(70, 70);
    return false;
  }
  else {
    halt();
    return true;
  }
}

bool DriveTrain::turn180(bool isRight) {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 950) {
    if (isRight) {
      turn(110, 110);
    }
    else {
      turn(70, 70);
    }
    return false;
  }
  else {
    halt();
    return true;
  }
}
