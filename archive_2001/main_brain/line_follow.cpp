/**
 ********************************************************************************************************
 * @file    line_follow.cpp
 * @brief   line follow methods
 * @details Executes various line following routines and follows crossing paths
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

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

// update digital sensor values
void LineFollow::updateSensorData() {
  currentSensors.left = (analogRead(_leftPort) > _whiteThreshold); // true if on black
  currentSensors.middle = (analogRead(_middlePort) > _whiteThreshold);
  currentSensors.right = (analogRead(_rightPort) > _whiteThreshold);
//  currentSensors.back = (analogRead(_backPort) > _whiteThreshold);
  
  // TODO make this so that it will check if sensors are new
}

int LineFollow::doLineFollowTillCross(DriveTrain& driving, DriveTrain::dirTravel dir) {
  // once we have all 3 sensors on a line, we have hit the cross, so stop
  int result = doLineFollow(driving, dir);
  if (result == 1)
    driving.halt();
  return result; // return 1 if we are on the cross, 0 otherwise
  
}

int LineFollow::doLineFollow(DriveTrain& driving, DriveTrain::dirTravel dir) {
  updateSensorData();
  // if we are not on the cross
  if (allOnCross() != 1) {
    // left sensor on the line
    if (currentSensors.left && !currentSensors.middle && !currentSensors.right){
      driving.turnLeft(dir);// turn left
      state = 0; // this is state 0
    }
    else {
      // right sensor on the line
      if (!currentSensors.left && !currentSensors.middle && currentSensors.right) {
        driving.turnRight(dir);// turn right
        state = 1; // this is state 1
      }
      else {
        if(!currentSensors.left && currentSensors.middle && !currentSensors.right) {
        // middle sensor on the line
        if (dir == DriveTrain::FORWARD)
          driving.forward();// go forward
        else
          driving.reverse(); // go reverse
        state = 2; // this is state 2
        }
        else{
          // we are off the line or have two tripped... What do we do?!?!?!
          // do the last state. That simple
          switch(state) {
            // we were going left, so turn left even more
            case 0:
                driving.sharpTurnLeft(dir);
              break;
            // we were going right, so turn right even more
            case 1:
              driving.sharpTurnRight(dir);
              break;
            // we were going forward last time, so just keep doing that
            case 2:
              if (dir == DriveTrain::FORWARD)  
                driving.forward();
              else
                driving.reverse();
              break;
            // if not this, then just go forward... you'll eventually go off the table if you're lucky
            default:
              state = 2;
              break;
          } // end switch
          
        } // end off the line
      }// end not on right sensor
    
    } // end not left sensor
  } // end not all sensors
  // we are on the cross...
  else if(goForwardOnCross) 
    driving.forward(); // if desired behavior is to keep going, do that.
    
  return allOnCross(); // return the state of them all on the cross
}

int LineFollow::allOnCross() {
  return (currentSensors.left && currentSensors.middle && currentSensors.right); // AND all the sensors
}

int LineFollow::stopOnCrossing(DriveTrain& driving, int number, DriveTrain::dirTravel dir) {
  goForwardOnCross = false; // we want to stop, so do not keep moving
  int currentRun = 0;      // the current run is 1 if on cross, 0 if not. used to not count the same line twice.
  if (sumCrossings < number) // if we have not gone over as many crossings as we want, keep moving
    currentRun = doLineFollow(driving, dir);
  if (currentRun == 1 && lastRunValue == 1) {
    // don't count this one, we're still on the same line
    // TODO - double check for noise - going on and off quickly
  }
  // otherwise, we have just hit a new line, so count it!
  else  
    sumCrossings += currentRun;
  // remember this run
  lastRunValue = currentRun;
  // if we have met the number of requested crossings, stop and return 1
  if (sumCrossings >= number) {
    driving.halt();
    return 1;
  }
  else // haven't made it yet, return 0
    return 0;
}

void LineFollow::resetCrossCount() {
  state = 0;
  lastRunValue = 0;
  sumCrossings = 0;
}

void LineFollow::setForwardOnCross(bool goesForward) {
  goForwardOnCross = goesForward;
}
