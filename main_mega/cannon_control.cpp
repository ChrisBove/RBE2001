/**
 ********************************************************************************************************
 * @file    cannon_control.cpp
 * @brief   cannon control class
 * @details Used to control cannon
 ********************************************************************************************************
 */

#include "Arduino.h"
#include "cannon_control.h"

#include <Servo.h>  // import libraries

Servo

CannonControl::CannonControl(int gripperPin, int servoPin, int motorPin, int flamePin, int encoderPin1, int encoderPin2) {
  _gripperPin = gripperPin;
  _servoPin = servoPin;
  _motorPin = motorPin;
  _flamePin = flamePin;
  _encoderPin1 = encoderPin1;
  _encoderPin2 = encoderPin2;
}


void CannonControl::setupCannon(){
  winch.attach(_motorPin, 1000, 2000);
  hinge.attach(_servoPin, 1000, 2000);
  grip.attach(_gripperPin, 1000, 2000);
}


