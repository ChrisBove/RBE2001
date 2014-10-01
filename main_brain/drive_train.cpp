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

void DriveTrain::AttachMotors() {
  left.attach(_leftPin);
  right.attach(_rightPin);
}

void DriveTrain::MoveMotors(int leftVal, int rightVal) {
  _leftDrive = leftVal;
  _rightDrive = rightVal;
  left.write(leftOffset - _leftDrive);
  right.write(_rightDrive);
}

void DriveTrain::turnLeft(){
  left.write(100); // should be slower or backwards
  right.write(65);
}

void DriveTrain::turnRight() {
  left.write(125);
  right.write(80);
}

void DriveTrain::halt(){
  left.write(leftStop);
  right.write(rightStop);
}

void DriveTrain::forward() {
  left.write(120);
  right.write(50); 
}
