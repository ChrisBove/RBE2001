#include "line_follow.h"
#include "drive_train.h"

LineFollow::LineFollow(int leftPort, int middlePort, int rightPort, int backPort) {
  _leftPort = leftPort;
  _middlePort = middlePort;
  _rightPort = rightPort;
  _backPort = backPort;
  
}

// runs during startup to calibrate values. Center sensor should be over the center line, outside left and right
// should be over the white
void LineFollow::calibrate() {
  int left = analogRead(_leftPort);
  int right = analogRead(_rightPort);
  _whiteThreshold = 3*((right + left)/2); // take average and multiply by 3. Consider this the threshold.
}

void LineFollow::setThreshold(int value) {
  _whiteThreshold = value;
}

// true if black
void LineFollow::updateSensorData() {
  currentSensors.left = (analogRead(_leftPort) > _whiteThreshold); // true if on black
  currentSensors.middle = (analogRead(_middlePort) > _whiteThreshold);
  currentSensors.right = (analogRead(_rightPort) > _whiteThreshold);
//  currentSensors.back = (analogRead(_backPort) > _whiteThreshold);
  
  // TODO make this so that it will check if sensors are new
}

// makes number from data
//int LineFollow::makeBinNumber() {
//  if (currentSensors.left
//}

int LineFollow::doLineFollowTillCross(DriveTrain& driving, DriveTrain::dirTravel dir) {
  int result = doLineFollow(driving, dir);
  if (result == 1)
    driving.halt();
  return result;
  
}

int LineFollow::doLineFollow(DriveTrain& driving, DriveTrain::dirTravel dir) {
  updateSensorData();
  if (allOnCross() != 1) {
    if (currentSensors.left && !currentSensors.middle && !currentSensors.right){
      driving.turnLeft(dir);// turn left
      state = 0;
    }
    else {
      if (!currentSensors.left && !currentSensors.middle && currentSensors.right) {
        driving.turnRight(dir);// turn right
        state = 1;
      }
      else {
        if(!currentSensors.left && currentSensors.middle && !currentSensors.right) {
        if (dir == DriveTrain::FORWARD)
          driving.forward();// go forward
        else
          driving.reverse();
        state = 2;
        }
        else{
          // do the last state
          switch(state) {
            case 0:
//              driving.turnLeft();
                driving.sharpTurnLeft(dir);
//              if (dir == DriveTrain::FORWARD)
//                driving.turn(80, 75);
//              else
//                driving.turn(110, 100);
              break;
            case 1:
//              driving.turnRight();
              driving.sharpTurnRight(dir);
//              if (dir == DriveTrain::FORWARD)
//                driving.turn(110, 100);
//              else
//                driving.turn(80, 75);
              break;
            case 2:
              if (dir == DriveTrain::FORWARD)  
                driving.forward();
              else
                driving.reverse();
              break;
            default:
              state = 2;
              break;
          }
          
        }
      }
    
    }
  }
  else if(goForwardOnCross) 
    driving.forward();
    
  return allOnCross();
}

int LineFollow::allOnCross() {
  return (currentSensors.left && currentSensors.middle && currentSensors.right);
  
}

int LineFollow::stopOnCrossing(DriveTrain& driving, int number, DriveTrain::dirTravel dir) {
  goForwardOnCross = false;
  int currentRun = 0;
  if (sumCrossings < number)
    currentRun = doLineFollow(driving, dir);
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
  state = 0;
  lastRun = 0;
  sumCrossings = 0;
}

void LineFollow::setForwardOnCross(bool goesForward) {
  goForwardOnCross = goesForward;
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

