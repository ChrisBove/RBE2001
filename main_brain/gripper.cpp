#include "Arduino.h"
#include "gripper.h"

#include <Servo.h>

Gripper::Gripper(gripperState state ) {
  thoughtState = state;
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

bool workArm(int state) {
  switch (state) {
    case Gripper::INIT_CGRIP:
      setReactTime();
      break;
    case Gripper::CGRIP:
      bool result = closeGrip();
      if (result) {
        break;
      }
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

bool closeGrip() {
  int timeLapse = millis() - reactTime;
  if (timeLapse <= 500) {
    grip.write(145);
    return true;
  }
  return false;
}
