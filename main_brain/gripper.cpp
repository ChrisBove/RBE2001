#include "Arduino.h"
#include "gripper.h"

#include <Servo.h>


Gripper::Gripper(int gripServo, int rackServo) {
  _gripServo = gripServo;
  _rackServo = rackServo;
}

void Gripper::attachMotors() {
  grip.attach(_gripServo);
  rack.attach(_rackServo);
  rack.write(90);
  grip.write(45);
}

void Gripper::extend(){
  rack.write(25);
}

void Gripper::retract(){
  rack.write(150);
}

void Gripper::openGrip(){
  grip.write(45);
}

void Gripper::closeGrip(){
  grip.write(145);
}

