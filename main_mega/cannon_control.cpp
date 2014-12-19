/**
 ********************************************************************************************************
 * @file    cannon_control.cpp
 * @brief   cannon control class
 * @details Used to control cannon
 ********************************************************************************************************
 */

#include "Arduino.h"
#include "cannon_control.h"

#include <Servo.h>
#include <Encoder.h>

#define ENCODER_PIN_1 18
#define ENCODER_PIN_2 23

const bool shouldDebug = false;

   Servo winch; 
   Servo hinge;
   Servo grip;

  Encoder canEnc(ENCODER_PIN_1, ENCODER_PIN_2);

CannonControl::CannonControl(int gripperPin, int servoPin, int motorPin, int flamePin) {
  _gripperPin = gripperPin;
  _servoPin = servoPin;
  _motorPin = motorPin;
  _flamePin = flamePin;
  _encoderPin1 = ENCODER_PIN_1;
  _encoderPin2 = ENCODER_PIN_2;
}

void CannonControl::resetCannon(){  // resets key values for repeated operation
  flameFound = false;
  servoPosition = 0;
  flameVal = 1000;
  currentFlameVal = 1000;
  //slackComp();
  locateFlameTrue = true;
  drawBackCont = true;
  state = LOCATE_FLAME;
}

void CannonControl::setupCannon(){  // sets up the cannon
  winch.attach(_motorPin);
  hinge.attach(_servoPin);
  grip.attach(_gripperPin);
  grip.write(90);
  servoPosition = servoMin;
  state = LOCATE_FLAME;
}

void CannonControl::checkFlame(){   // compares the current values of the flame sensor and adjustes them acodingly
  currentFlameVal = analogRead(_flamePin);
  if(flameVal >= currentFlameVal){
    flameVal = currentFlameVal;
    currentFlamePos = servoPosition;
    //Serial.println(flameVal);
  }
}

void CannonControl::locateFlame(){   // scans for the canndle flame
  //flameFound = false;
//  if(cannonStart){
//    servoPosition = servoMin;
//    cannonStart = false;
//  }
  if(servoPosition <= servoMax)
  {
    hinge.write(servoPosition);
    checkFlame();  // checks the current flame values and adjust them accordingly
    if(counter >=100){
    servoPosition ++;
    if(shouldDebug) Serial.println(servoPosition);
    counter = 0;
    }
    else{
      counter ++;
    }
  }
  if(servoPosition >= servoMax){  // when the cannon reaches its max angle determines if the flame was found or not
    if(flameVal <= threshold){
      flameFound = true;
    }
    if(flameFound){              // flame was found
      state = DRAW_BACK;
//    locateFlameTrue = false;
//    drawBackTrue = true;
    if(shouldDebug) Serial.println("Check Flame");
//    Serial.println(servoPosition);
    //AIM();
    }
    if(flameVal >= threshold){ // flame was not found
      extinguished = true;
    }
//    else{
//     servoPosition = servoMin;
//    }
  }
}

void CannonControl::AIM(){  // rotates the cannon to the correct fireing position
  if(flameFound && aimCount <= 10000){
  hinge.write(currentFlamePos - 7);
  aimCount ++;
  }
  else{
    aimCount = 0;
    if(shouldDebug) Serial.println("AIM");
//    AIMTrue = false;
//    shootCannonTrue = true;
state = SHOOT_CANNON;
}
}

void CannonControl::drawBack(){    // winds up the winch
  //Serial.println("draw back start");
  //newPosition = -canEnc.read();
  //hinge.write(servoMax);
  if(newPosition <= 1210 && drawBackCont){
    newPosition = -canEnc.read();
    winch.write(120);
    if(newPosition != oldPosition){
    oldPosition = newPosition;
    //Serial.println(newPosition);
  }
  }
  if(newPosition > 1200){
    drawBackCont = false;
    winch.write(90);
    grip.write(180);
    gripClosed ++;
    if(gripClosed >= 1000){
      if(shouldDebug) Serial.println("draw back");
//    drawBackTrue = false;
//    giveSlackTrue = true;
state = GIVE_SLACK;
    }
  }
}

void CannonControl::slackComp(){  // not implemented
  if(num <= 5){
    winch.write(120);
    num ++;
  }
}

void CannonControl::giveSlack(){  // gives the initial slack in the line before fireing
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
    if(shouldDebug) Serial.println("giveslack");
//    giveSlackTrue = false;
//    giveTugTrue = true;
state = TUG;
}
}

void CannonControl::giveTug(){  // rotates the hinge servo giving more slack to the line
  if(servoPosition >= servoMin)
  {
    hinge.write(servoPosition);
    if(counter >=100){
    servoPosition--;
    counter = 0;
    }
    else{
      counter ++;
    }
    if(shouldDebug) Serial.println(servoPosition);
  }
  if(servoPosition <= servoMin){
//    giveTugTrue = false;
//    AIMTrue = true;
state = AIM_AT_FLAME;
    if(shouldDebug) Serial.println("Give Tug");
  }
}

void CannonControl::shootCannon(){
  if (gripClosed >= 0){ // opens the gripper
  grip.write(90);
  gripClosed --;
  }
  else{
    resetCannon();
    if(shouldDebug) Serial.println("shootcannon");
//  shootCannonTrue = false;
state = LOCATE_FLAME;
  }
}

int CannonControl::giveAngle(){ // calculates the angle of the cannon
   return currentFlamePos/5.71;
}

bool CannonControl::returnResult(){  // returns the current state of the candle
  return extinguished;
}

void CannonControl::service(){ // not implemented
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

void CannonControl::cannonOP(){
  switch (state) {
    
    case LOCATE_FLAME:
    locateFlame();
    break;
    
    case DRAW_BACK:
    drawBack();
    break;
    
    case GIVE_SLACK:
    giveSlack();
    break;
    
    case TUG:
    giveTug();
    break;
    
    case AIM_AT_FLAME:
    AIM();
    break;
    
    case SHOOT_CANNON:
    shootCannon();
    break;
    
  }
}


