/**
 ********************************************************************************************************
 * @file    virtual_bumper.cpp
 * @brief   virtual bumper class
 * @details takes range sensor data and makes it into a virtual bumper and steers the robot around obstacles
 ********************************************************************************************************
 */
 
#include "Arduino.h"
#include "virtual_bumper.h"
#include "cliff.h"
 
#define LEFT_CLIFF       10
#define RIGHT_CLIFF      11
#define LEFT_IR          4
#define RIGHT_IR         3

const bool shouldPrint = false;

CliffDetector cliffDetect(RIGHT_CLIFF, LEFT_CLIFF);

VirtualBumper::VirtualBumper(int ultraPin, int digUltraPin) {
  _ultraPin = ultraPin;
  _digUltraPin = digUltraPin;
}

void VirtualBumper::setupBumper() {
  pinMode(_digUltraPin, INPUT);
  
}

void VirtualBumper::service() {
  // read all sensors
//  getAnalogDistance();
  // if angle is within forward direction, use the distance to update obstacle for forward direction
}

VirtualBumper::DIR VirtualBumper::cliffObstacle() {
  // save sensor states
  bool right = cliffDetect.fallingR();
  bool left = cliffDetect.fallingL();
  
  // check the sensor states and declare obstacle locations
  if (!right && !left )
    return NONE;
  if (right && !left)
    return RIGHT;
  if (!right && left )
    return LEFT;
  if (right && left )
    return L_AND_R;
  // otherwise
    return ALL;
}

VirtualBumper::DIR VirtualBumper::obstacle() {
  // save sensor states
  bool right = seeObstacleIR(R);
  bool left = seeObstacleIR(L);
  bool center = seeObstacleUS();
  
  // check the sensor states and declare obstacle locations
  if (!right && !left && !center)
    return NONE;
  if (right && !left && !center)
    return RIGHT;
  if (!right && left && !center)
    return LEFT;
  if (!right && !left && center)
    return CENTER;
  if (right && left && !center)
    return L_AND_R;
  if (right && !left && center)
    return CENTER_R;
  if (!right && left && center)
    return CENTER_L;
  if (right && left && center)
    return ALL;
  // otherwise
  return ALL;
}

bool VirtualBumper::seeObstacleIR(SIDE side) {
  if (side == R)
    return analogRead(RIGHT_IR) > sharpThreshold;
  else
    return analogRead(LEFT_IR) > sharpThreshold;
}

bool VirtualBumper::seeObstacleUS() {
  return getAnalogDistance() <= ultraThreshold;
}

int VirtualBumper::getAnalogDistance() {
  reading = 1+analogRead(_ultraPin)/2; // divide by 2: 4.9mV/2cm
  return reading;
}

int VirtualBumper::getDigitalDistance() {
  distance = pulseIn(_digUltraPin, HIGH)/58; // 
  return distance;
}

int VirtualBumper::getDistance() {
  return reading;
}

void VirtualBumper::steerMe(DriveTrain& drive) {
  // check if we have hit a cliff - if so, backup and spin for a bit
  bool onCliff = true;
  
  
  if (!stillOnCliff) // if we are not still servicing a cliff event, check them again
    cliffDir = cliffObstacle();
  // otherwise, just remember our last direction
  switch(cliffDir) {
    case NONE:
      // we're good on cliff stuff, check your bumpers
      if(shouldPrint) Serial.println("No cliff");
      onCliff = false;
      stillOnCliff = false;
      break;
    
    case RIGHT:
      if(shouldPrint) Serial.println("right cliff");
      stillOnCliff = true;
      // backup
      if (!backupDone) // if not done
        backupDone = drive.backupX(4); // backup 4 inches
      if (backupDone) {
        if(drive.rotateX(1.57)) {// rotate left by 90 degrees, +1.57 radians 
          // if done moving, update stillOnCliff and onCliff
          stillOnCliff = false;
          backupDone = false;
        }
      }
      break;
    
    case LEFT:
      if(shouldPrint) Serial.println("left cliff");
      stillOnCliff = true;
      // backup
      if (!backupDone) // if not done
        backupDone = drive.backupX(4); // backup 4 inches
      if (backupDone) {
        if(drive.rotateX(-1.57)) {// rotate right by 90 degrees, -1.57 radians
          stillOnCliff = false;
          backupDone = false;
        }
      }
      break;
      
    case L_AND_R:
      if(shouldPrint) Serial.println("both cliff");
      stillOnCliff = true;
      // backup
      if (!backupDone) // if not done
        backupDone = drive.backupX(4); // backup 4 inches
      if (backupDone) {
        if(drive.rotateX(PI)) {// rotate left by 180, +PI radians
          stillOnCliff = false;
          backupDone = false;
        }
      }
      break;
      
    default:
      // shouldn't run
      onCliff = false;
      stillOnCliff = false;
      break;
  }
  
  // if not on a cliff, then service our bumper stuffs
  if(!onCliff) {
    if (!stillBumped)
      bumperDir = obstacle();
      
    switch (bumperDir) {
      case NONE:
        if(shouldPrint) Serial.println("No obstacle");
        // otherwise, move forward unless there is an object on our bumper
        drive.moveMotors(30,30); // go forward
        stillBumped = false;
        break;
      
      case RIGHT:
        if(shouldPrint) Serial.println("Right obstacle");
        stillBumped = true;
        // backup a bit
        if(drive.rotateX(0.52))// rotate left 30 degrees, 0.52 radians
          stillBumped = false;
        break;
      
      case LEFT:
        if(shouldPrint) Serial.println("left obstacle");
        stillBumped = true;
        // backup a bit
        if(drive.rotateX(-0.52))// rotate right 30 degrees, -0.52
          stillBumped = false;
        break;
        
      case CENTER:
        if(shouldPrint) Serial.println("center obstacle");
        stillBumped = true;
        // backup a bit
        if(drive.rotateX(1.57))// rotate left 90 degrees, 1.57
          stillBumped = false;
        break;
      
      case CENTER_R:
        if(shouldPrint) Serial.println("c R obstacle");
        stillBumped = true;
        // backup a bit
        if(drive.rotateX(1.05))// rotate left 60 degrees, 1.05
          stillBumped = false;
        break;
      
      case CENTER_L:
        if(shouldPrint) Serial.println("c l obstacle");
        stillBumped = true;
        // backup a bit
        if(drive.rotateX(-1.05))// rotate right 60 degrees, -1.05
          stillBumped = false;
        break;
      
      case L_AND_R:
        if(shouldPrint) Serial.println("l and r obstacle");
        // that just happened?
        stillBumped = true;
        // backup a bit more
        if(drive.rotateX(1.57))// rotate left 90 degrees, 1.57
          stillBumped = false;
        break;
       
      case ALL:
        if(shouldPrint) Serial.println("all obstacle");
        // weep quietly
        stillBumped = true;
        if(drive.rotateX(PI))// rotate left 180 degrees, PI radians
          stillBumped = false;
        break;
      
      default:
        stillBumped = false;
        break;
    }
  } // end if not on cliff
  
}

void VirtualBumper::wallFollow(DriveTrain& drive) {
  
}
