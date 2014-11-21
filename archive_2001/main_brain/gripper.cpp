/**
 ********************************************************************************************************
 * @file    gripper.cpp
 * @brief   gripper control methods
 * @details Used to control the gripper's claw and extension
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "gripper.h"

#include <Servo.h>

Gripper::Gripper(gripperState state ) {
}

Gripper::Gripper(int gripServo, int rackServo) {
  _gripServo = gripServo;
  _rackServo = rackServo;
}

void Gripper::attachMotors() {
  grip.attach(_gripServo);
  rack.attach(_rackServo);
  rack.write(50);  // extend
  grip.write(45);  // open
}

void Gripper::setReactTime() {
  reactTime = millis();
}

bool Gripper::closeTheGrip() {
  // switch based on a state
  // initialize the time, then close until the time has passed.
  switch (grippyStateClose) {
    case INIT_CLOSE_GRIP:
      setReactTime();
      grippyStateClose = CLOSE_GRIP;
      return false;
      break;
    case CLOSE_GRIP:
      bool result = closeGrip();
      if (result) {
        grippyStateClose = INIT_CLOSE_GRIP;
        return true;
      }
      return false;
      break;
  }
}

bool Gripper::retractTheGrip() {
  switch (grippyStateRetract) {
    case INIT_RETRACT:
      setReactTime();
      grippyStateRetract = RETRACT;
      return false;
      break;
    case RETRACT:
      bool result = retract();
      if (result) {
        grippyStateRetract = INIT_RETRACT;
        return true;
      }
      return false;
      break;
  }
}

bool Gripper::extendTheGrip() {
  switch (grippyStateExtend) {
    case INIT_EXTEND:
    setReactTime();
    grippyStateExtend = EXTEND;
    return false;
    break;
    case EXTEND:
    bool result = extend();
    if (result) {
      grippyStateExtend = INIT_EXTEND;
      return true;
    }
    return false;
    break;
  }
}

bool Gripper::extendLimTheGrip() {
  switch (grippyStateExtendLim) {
    case INIT_EXTENDLIM:
    setReactTime();
    grippyStateExtendLim = EXTENDLIM;
    return false;
    break;
    case EXTENDLIM:
    bool result = extendLim();
    if (result) {
      grippyStateExtendLim = INIT_EXTENDLIM;
      return true;
    }
    return false;
    break;
  }
}

bool Gripper::openTheGrip() {
  switch (grippyStateOpen) {
    case INIT_OPEN_GRIP:
    setReactTime();
    grippyStateOpen = OPEN_GRIP;
    return false;
    break;
    case OPEN_GRIP:
    bool result = openGrip();
    if (result) {
      grippyStateOpen = INIT_OPEN_GRIP;
      return true;
    }
    return false;
    break;
  }
}
    
bool Gripper::extend() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 1000) {
    rack.write(15);
    return false;
  }
  return true;

}

bool Gripper::extendLim() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 1000) {
    rack.write(50);
    return false;
  }
  return true;
}

bool Gripper::retract() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 1000) {
    rack.write(180);
    return false;
  }
  return true;
}

bool Gripper::openGrip() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 500) {
    grip.write(45);
    return false;
  }
  return true;
}

bool Gripper::closeGrip() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 500) {
    grip.write(150);
    return false;
  }
  return true;
}
