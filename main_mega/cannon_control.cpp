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

#define GRIPPER_PIN  8
#define SERVO_PIN    7
#define MOTOR_PIN    6
#define FLAME_PIN    A2
#define ENCODER_PIN_1 A3
#define ENCODER_PIN_2 A4

CannonControl::CannonControl(int gripperPin, int servoPin, int motorPin, int flamePin, int encoderPin1, int encoderPin2) {
  _gripperPin = gripperPin;
  _servoPin = servoPin;
  _motorPin = motorPin;
  _flamePin = flamePin;
  _encoderPin1 = encoderPin1;
  _encoderPin2 = encoderPin2;
}


void CannonControl::setupCannon(){
  winch.attach(_motorPin);
  hinge.attach(_servoPin, 1000, 2000);
  grip.attach(_gripperPin);
}

void CannonControl::checkFlame(){
  currentFlameVal = analogRead(_flamePin);
  if(flameVal >= currentFlameVal){
    flameVal = currentFlameVal;
    currentFlamePos = servoPos;
  }
}

void CannonControl::locateFlame(){
  if(servoPos <= servoMax)
  {
    hinge.write(servoPos);
    checkFlame();
    delay(100); 
  }
  if(servoPos >= servoMax){
    flameFound = true;
  }
}

void CannonControl::AIM(){
  if(flameFound){
  hinge.write(currentFlamePos);
  }
}


