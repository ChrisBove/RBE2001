#include "Arduino.h"
#include "drive_train.h"

#include <Servo.h>  // import libraries
#include <PPM.h> // includes the PPM library\


DriveTrain::DriveTrain(int leftPin, int rightPin, bool leftInverted, bool rightInverted) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  
  left.attach(_leftPin);
  right.attach(_rightPin);
  
  if (leftInverted)
    leftOffset = 180;
  if (rightInverted)
    rightOffset = 180;
  
}

void DriveTrain::MoveMotors(int leftVal, int rightVal) {
  left.write(leftOffset - leftVal);
  right.write(rightOffset - rightVal);
}


