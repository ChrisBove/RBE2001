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
  currentSensors.left = (analogRead(_leftPort) > _white); // true if on black
  currentSensors.middle = (analogRead(_middlePort) > _white);
  currentSensors.right = (analogRead(_rightPort) > _white);
  currentSensors.back = (analogRead(_backPort) > _white);
  
  // TODO make this so that it will check if sensors are new
}

// makes number from data
//int LineFollow::makeBinNumber() {
//  if (currentSensors.left
//}

int LineFollow::doLineFollowTillCross(DriveTrain& driving) {
  int result = doLineFollow(driving);
  if (result == 1)
    driving.halt();
  return result;
  
}

int LineFollow::doLineFollow(DriveTrain& driving) {
  updateSensorData();
  static int state;
  if (allOnCross() != 1) {
    if (currentSensors.left && !currentSensors.middle && !currentSensors.right){
      driving.turnLeft();// turn left
      state = 0;
    }
    else {
      if (!currentSensors.left && !currentSensors.middle && currentSensors.right) {
        driving.turnRight();// turn right
        state = 1;
      }
      else {
        if(!currentSensors.left && currentSensors.middle && !currentSensors.right) {
        
        driving.forward();// go forward
        state = 2;
        }
        else{
          // do the last state
          switch(state) {
            case 0:
//              driving.turnLeft();
              driving.turn(80, 75);
              break;
            case 1:
//              driving.turnRight();
              driving.turn(110, 100);
              break;
            case 2:
              driving.forward();
              break;
            default:
              state = 2;
              break;
          }
          
        }
      }
    
    }
  }
  return allOnCross();
}

int LineFollow::allOnCross() {
  return (currentSensors.left && currentSensors.middle && currentSensors.right);
  
}

int LineFollow::stopOnCrossing(DriveTrain& driving, int number) {
  int currentRun = 0;
  if (sumCrossings < number)
    currentRun = doLineFollow(driving);
  if (currentRun == 1 && lastRun == 1) {
    // don't count this one, we're still on the same line
    // TODO - double check for noise - going on and off quickly
  }
  else  
    sumCrossings += currentRun;
    
  lastRun = currentRun;
  if (sumCrossings >= number) {
    driving.halt();
    return 1;
  }
  else
    return 0;
}

void LineFollow::resetCrossCount() {
  lastRun = 0;
  sumCrossings = 0;
}
//// motorspeed data
//void DriveTrain::turnLeft(){
//  lSpeed = 150;
//  rSpeed = 0;
//}
//
//void DriveTrain::turnRight() {
//  lSpeed = 180;
//  rSpeed = 30;
//}
//
//void DriveTrain::halt(){
//  lSpeed = 90;
//  rSpeed = 90;
//}
//
//void DriveTrain::forward() {
//  lSpeed = 180;
//  rSpeed = 0; 
//}
