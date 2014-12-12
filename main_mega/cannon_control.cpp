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
#include <Encoder.h>

#define ENCODER_PIN_1 10
#define ENCODER_PIN_2 23

  Encoder canEnc(ENCODER_PIN_1, ENCODER_PIN_2);

CannonControl::CannonControl(int gripperPin, int servoPin, int motorPin, int flamePin) {
  _gripperPin = gripperPin;
  _servoPin = servoPin;
  _motorPin = motorPin;
  _flamePin = flamePin;
  _encoderPin1 = ENCODER_PIN_1;
  _encoderPin2 = ENCODER_PIN_2;
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
    drawBackTrue = true;
  }
}

void CannonControl::AIM(){
  if(flameFound){
  hinge.write(currentFlamePos);
  }
}

void CannonControl::drawBack(){
  newPosition = canEnc.read();
  if(counter <= 100){
    if(newPosition != oldPosition){
    oldPosition = newPosition;
    counter = oldPosition + counter;
    winch.write(120);
  }
  }
  if(counter >= 100){
    winch.write(90);
    grip.write(180);
  }
}

void CannonControl::giveSlack(){
  newPosition = canEnc.read();
  if(oldPosition >= 0 && newPosition != oldPosition){
    oldPosition = newPosition;
    winch.write(80);
  }
}

void CannonControl::shootCannon(){
  grip.write(90);
}
