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

void CannonControl::resetCannon(){
  flameFound = false;
  servoPos = 0;
  flameVal = 1000;
  currentFlameVal = 1000;
  //slackComp();
  locateFlameTrue = true;
  drawBackCont = true;
}

void CannonControl::setupCannon(){
  winch.attach(_motorPin);
  hinge.attach(_servoPin);
  grip.attach(_gripperPin);
  grip.write(90);
  //servoPos = servoMin;
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
  //flameFound = false;
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
    Serial.println("Check Flame");
    Serial.println(servoPos);
    //AIM();
    }
    if(flameVal >= threshold){
      extinguished = true;
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
    Serial.println("AIM");
    AIMTrue = false;
    shootCannonTrue = true;
}
}

void CannonControl::drawBack(){
  //Serial.println("draw back start");
  //newPosition = -canEnc.read();
  //hinge.write(servoMax);
  if(newPosition <= 1010 && drawBackCont){
    newPosition = -canEnc.read();
    winch.write(120);
    if(newPosition != oldPosition){
    oldPosition = newPosition;
    //Serial.println(newPosition);
  }
  }
  if(newPosition > 1000){
    drawBackCont = false;
    winch.write(90);
    grip.write(180);
    gripClosed ++;
    if(gripClosed >= 20){
      Serial.println("draw back");
    drawBackTrue = false;
    giveSlackTrue = true;
    }
  }
}

void CannonControl::slackComp(){
  if(num <= 5){
    winch.write(120);
    num ++;
  }
}

void CannonControl::giveSlack(){
  //newPosition = -canEnc.read();
  if(newPosition >= 70){
    newPosition = -canEnc.read();
    winch.write(60);
  if(newPosition != oldPosition){
    oldPosition = newPosition;
    //Serial.println(newPosition);
  }
  }
  if(newPosition < 70){
    winch.write(90);
    Serial.println("giveslack");
    giveSlackTrue = false;
    giveTugTrue = true;
}
}

void CannonControl::giveTug(){
  if(servoPos >= servoMin)
  {
    hinge.write(servoPos);
    if(counter >=3){
    servoPos--;
    counter = 0;
    }
    else{
      counter ++;
    }
    Serial.println(servoPos);
  }
  if(servoPos <= servoMin){
    giveTugTrue = false;
    AIMTrue = true;
    Serial.println("Give Tug");
  }
}

void CannonControl::shootCannon(){
  if (gripClosed >= 0){
  grip.write(90);
  gripClosed --;
  }
  else{
    resetCannon();
    Serial.println("shootcannon");
  shootCannonTrue = false;
  }
}

int CannonControl::giveAngle(){ //19cm high 20 cm out, 28 degrees at max, 1.777 ratio
   return currentFlamePos/5.71;
}

bool CannonControl::returnResult(){
  return extinguished;
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
  if(giveTugTrue){
    giveTug();
  }
  if(AIMTrue){
    AIM();
  }
  if(shootCannonTrue){
    shootCannon();
  }
}
