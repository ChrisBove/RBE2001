#include "arm.h"
#include "button.h"

Servo arm;


Arm::Arm(int motorPin, int potPin) {
  _motorPin = motorPin;
  _potPin = potPin;
}
    
void Arm::setupArm() {
  arm.attach(_motorPin);
  arm.write(90);
//  Serial.begin(115200);
}

void Arm::setPosition(int setPoint, Button& frontLimit) {
  int desPos = setPoint;  // desired input: down is +, up - negative
  int curPos = analogRead(_potPin);  // sensor input to degrees
  
  int currentError = desPos - curPos; // calculate the current error
//  Serial.print("currentError: "); // debug print
//  Serial.print(currentError);
//  Serial.print("\t");  
  
  //////// do output calculation - the main thing! /////////
  float output = Kp*(currentError) + Ki*(errorSum) + Kd*(currentError-lastError);
  
//  Serial.print("Output: "); // debug print
//  Serial.print(output);
//  Serial.print("\t"); 
//  delay(10); // prevent spamming serial
  
  // if we're trying to move down and we hit the switch,
  if (currentError < 0 && frontLimit.isBumped()) {
    goodness++;
    sendOutput(0, frontLimit);
  }
  else if (abs(currentError) > 10 ) {
    sendOutput(output, frontLimit); // send output
    goodness = 0;
  }
  else {
    sendOutput(0, frontLimit); // otherwise, set to stop
    goodness++;
  }
    
  // increment errorSum if not too big/long
  if (count < 100) {
    errorSum += currentError; // add error to sum
    count++; // increment count
  }
  else {
    errorSum = currentError; // reset error sum to current
    count = 0; // reset count
  }
//  Serial.print("errorSum: "); // debug
//  Serial.print(errorSum);  
//  Serial.print("\t"); 
    
  // set last error equal to this error
  lastError = currentError;
}

// takes output from PWM and sends it through to the motors
void Arm::sendOutput(float output, Button& frontLimit) {
  // if output is not 0
  if (abs(output) > 0) {
    // if output is negative, rotate up
    if (output < 0) {
      rotateUp(scaleOutput(output), frontLimit);
    }
    else {
      // otherwise, go down
      rotateDown(scaleOutput(output), frontLimit);
    }
  }
  // otherwise, we are perfect, so stop the motor
  else
    arm.write(90);
}

// scales a passed output to prevent motor stalls using PWM and analog write
float Arm::scaleOutput (float output) {
  // adds a bit to prevent stall, scales to 255
  float result = 10+abs(1000*output);
//  Serial.print("scaled: "); // print for debug
//  Serial.print(result);  
//  Serial.println("\t"); 
  return result;
}

bool Arm::isInPosition() {
  if (goodness > 200)
    return true;
  return false;
}

void Arm::rotateUp (float output, Button& frontLimit){
  if (!frontLimit.isBumped())
    arm.write(70+output);
  else {
    arm.write(90);
    goodness++;
  }
}

void Arm::rotateDown (float output, Button& frontLimit) {
  arm.write(90-output);
}


bool Arm::goUp(Button& frontLimit) {
  setPosition(696, frontLimit);
  return isInPosition();
}

bool Arm::goDown(Button& frontLimit) {
  setPosition(330, frontLimit);
  return isInPosition();
}
// positive values put the arm down

// 342 is lowest position
// 696 is highest position.


