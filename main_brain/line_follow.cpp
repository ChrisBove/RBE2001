#include "line_follow.h"
#include "drive_train.h"

LineFollow::LineFollow(int leftPort, int middlePort, int rightPort, int backPort) {
  _leftPort = leftPort;
  _middlePort = middlePort;
  _rightPort = rightPort;
  _backPort = backPort;
}

void LineFollow::setThreshold(int value) {
  _white = value;
}

// true if black
void LineFollow::updateSensorData() {
  currentSensors.left = analogRead(_leftPort) < _white;
  currentSensors.middle = analogRead(_middlePort) < _white;
  currentSensors.right = analogRead(_rightPort) < _white;
  currentSensors.back = analogRead(_backPort) < _white;
  
  // TODO make this so that it will check if sensors are new
}

// makes number from data
//int LineFollow::makeBinNumber() {
//  if (currentSensors.left
//}

int LineFollow::doLineFollowTillCross() {
  updateSensorData();
  
  if (allOnCross() != 1) {
  if (currentSensors.left && !currentSensors.middle && !currentSensors.right)
    ;// turn left
  else {
    if (!currentSensors.left && !currentSensors.middle && currentSensors.right) {
      // turn right
    }
    else 
      // go forward
  }
  
  }
  else
    // stop robot
  return allOnCross();
  
}

int LineFollow::allOnCross() {
  return currentSensors.left && currentSensors.middle && currentSensors.right;
  
}


