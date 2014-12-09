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

//#define gripperPin  8
//#define servoPin    7
//#define motorPin    6
//#define flamePin    A2
//#define encoderPin1 A3
//#define encoderPin2 A4

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

void CannonControl::checkFlame(){
  currentFlameVal = analogRead(_flamePin);
  if(flameVal >= currentFlameVal){
    flameVal = currentFlameVal;
  }
}

void CannonControl::locateFlame(){
  for(servoPos = servoMin; servoPos <= servoMax; servoPos += 1)
  {
    hinge.write(servoPos);
    delay(20);
      
  }
}
