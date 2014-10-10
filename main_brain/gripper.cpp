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
  rack.write(25);
  grip.write(45);
}

void Gripper::setReactTime() {
  reactTime = millis();
}

bool Gripper::closeTheGrip() {
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

bool Gripper::openTheGrip() {
  switch (grippyStateOpen) {
    case INIT_OPEN_GRIP:
    setReactTime();
    grippyStateClose = OPEN_GRIP;
    return false;
    break;
    case OPEN_GRIP:
    bool result = openGrip();
    if (result) {
      grippyStateExtend = INIT_OPEN_GRIP;
      return true;
    }
    return false;
    break;
  }
}
    
bool Gripper::extend() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 1000) {
    rack.write(25);
    return true;
  }
  return false;
}

bool Gripper::retract() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 1000) {
    rack.write(150);
    return true;
  }
  return false;
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
    grip.write(145);
    return false;
  }
  return true;
}
