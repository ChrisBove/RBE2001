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

// 2.3V for 10cm object, 2.75V for 8cm object, 3.15V for 6cm and below 6 cm obstacles voltage again decreases
const int threshold = 563; // object if greater than this threshold

VirtualBumper::VirtualBumper(int ultraPin, int digUltraPin) {
  _ultraPin = ultraPin;
  _digUltraPin = digUltraPin;
}

void VirtualBumper::setupBumper() {
  pinMode(_digUltraPin, INPUT);
  
}

void VirtualBumper::service() {
  // read all sensors
  getAnalogDistance();
  // if angle is within forward direction, use the distance to update obstacle for forward direction
}

VirtualBumper::DIR VirtualBumper::obstacle() {
  // create a state with all the sensor binary information
  // return the direction associated with that state
//  char state = seeObstacle + 
//  if (side == SIDE::right) {
//    
//  }
}

bool VirtualBumper::seeObstacle(SIDE side) {
  if (side == R)
    return analogRead(LEFT_IR) > threshold;
  else
    return analogRead(RIGHT_IR) > threshold;
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
  
}
