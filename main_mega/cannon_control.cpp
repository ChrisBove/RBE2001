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

#define ENCODER_PIN_1 18
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
  hinge.attach(_servoPin);
  grip.attach(_gripperPin);
  grip.write(90);
}

void CannonControl::checkFlame(){
  currentFlameVal = analogRead(_flamePin);
  if(flameVal >= currentFlameVal){
    flameVal = currentFlameVal;
    currentFlamePos = servoPos;
    //Serial.println(flameVal);
  }
}

void CannonControl::locateFlame(){
  flameFound = false;
//  if(cannonStart){
//    servoPos = servoMin;
//    cannonStart = false;
//  }
  if(servoPos <= servoMax)
  {
    hinge.write(servoPos);
    checkFlame();
    if(counter >=3){
    servoPos++;
    counter = 0;
    }
    else{
      counter ++;
    }
  }
  if(servoPos >= servoMax){
    if(flameVal <= threshold){
      flameFound = true;
    }
    if(flameFound){
    locateFlameTrue = false;
    drawBackTrue = true;
    //AIM();
    }
//    else{
//     servoPos = servoMin;
//    }
  }
}

void CannonControl::AIM(){
  if(flameFound && aimCount <= 100){
  hinge.write(currentFlamePos - 10);
  aimCount ++;
  }
  else{
    aimCount = 0;
    AIMTrue = false;
    shootCannonTrue = true;
}
}

void CannonControl::drawBack(){
  //newPosition = -canEnc.read();
  //hinge.write(servoMax);
  if(newPosition <= 1000 && drawBackCont){
    newPosition = -canEnc.read();
    winch.write(120);
    if(newPosition != oldPosition){
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  }
  if(newPosition > 1000){
    drawBackCont = false;
    winch.write(90);
    grip.write(180);
    gripClosed ++;
    if(gripClosed >= 20){
    drawBackTrue = false;
    giveSlackTrue = true;
    }
  }
}

void CannonControl::giveSlack(){
  //newPosition = -canEnc.read();
  if(newPosition >= 0){
    newPosition = -canEnc.read();
    winch.write(60);
  if(newPosition != oldPosition){
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  }
  if(newPosition < 0){
    winch.write(90);
    giveSlackTrue = false;
    AIMTrue = true;
}
}

void CannonControl::shootCannon(){
  if (gripClosed >= 0){
  grip.write(90);
  gripClosed --;
  }
  else{
  shootCannonTrue = false;
  servoPos = servoMin;
  locateFlameTrue = true;
  }
}

void CannonControl::service(){
  if(locateFlameTrue){
    locateFlame();
  }
  if(drawBackTrue){
    drawBack();
  }
  if(giveSlackTrue){
    giveSlack();
  }
  if(AIMTrue){
    AIM();
  }
  if(shootCannonTrue){
    shootCannon();
  }
}
