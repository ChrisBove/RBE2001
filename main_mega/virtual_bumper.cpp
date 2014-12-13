/**
 ********************************************************************************************************
 * @file    virtual_bumper.cpp
 * @brief   virtual bumper class
 * @details takes range sensor data and makes it into a virtual bumper and steers the robot around obstacles
 ********************************************************************************************************
 */
 
#include "Arduino.h"
#include "virtual_bumper.h"
#include "cliff.h"
 
#define LEFT_CLIFF       10
#define RIGHT_CLIFF      11
#define LEFT_IR          4
#define RIGHT_IR         3

CliffDetector cliffDetect(RIGHT_CLIFF, LEFT_CLIFF);

VirtualBumper::VirtualBumper(int ultraPin, int digUltraPin) {
  _ultraPin = ultraPin;
  _digUltraPin = digUltraPin;
}

void VirtualBumper::setupBumper() {
  pinMode(_digUltraPin, INPUT);
  
}

void VirtualBumper::service() {
  // read all sensors
//  getAnalogDistance();
  // if angle is within forward direction, use the distance to update obstacle for forward direction
}

VirtualBumper::DIR VirtualBumper::obstacle() {
  // save sensor states
  bool right = seeObstacleIR(R);
  bool left = seeObstacleIR(L);
  bool center = seeObstacleUS();
  
  // check the sensor states and declare obstacle locations
  if (!right && !left && !center)
    return NONE;
  if (right && !left && !center)
    return RIGHT;
  if (!right && left && !center)
    return LEFT;
  if (!right && !left && center)
    return CENTER;
  if (right && left && !center)
    return L_AND_R;
  if (right && !left && center)
    return CENTER_R;
  if (!right && left && center)
    return CENTER_L;
  if (right && left && center)
    return ALL;
}

bool VirtualBumper::seeObstacleIR(SIDE side) {
  if (side == R)
    return analogRead(LEFT_IR) > sharpThreshold;
  else
    return analogRead(RIGHT_IR) > sharpThreshold;
}

bool VirtualBumper::seeObstacleUS() {
  return getAnalogDistance() < ultraThreshold;
}

int VirtualBumper::getAnalogDistance() {
  reading = 1+analogRead(_ultraPin)/2; // divide by 2: 4.9mV/2cm
  return reading;
}

int VirtualBumper::getDigitalDistance() {
  distance = pulseIn(_digUltraPin, HIGH)/58; // 
  return distance;
}

int VirtualBumper::getDistance() {
  return reading;
}

void VirtualBumper::steerMe(DriveTrain& drive) {
  // check if we have hit a cliff - if so, backup and spin for a bit
  
  // otherwise, move forward unless there is an object on our bumper
  
  
}

void VirtualBumper::wallFollow(DriveTrain& drive) {
  
}
