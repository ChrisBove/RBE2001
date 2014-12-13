/**
 ********************************************************************************************************
 * @file    sensor_mast.cpp
 * @brief   sensor mast control and sensing methods
 * @details Used to control the sensor mast - reading sensors and turning servo
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "sensor_mast.h"

#include <Servo.h>  // import libraries

Servo servo;

SensorMast::SensorMast(int servoPin, int flamePin, int LED_indicator, int LED_WIN) {
  _servoPin = servoPin;
  _flamePin = flamePin;
  _LED_indicator = LED_indicator;
  _LED_WIN = LED_WIN;
}

void SensorMast::setupMast() {
  servo.attach(_servoPin); 
  // see http://makezine.com/2014/04/23/arduinos-servo-library-angles-microseconds-and-optional-command-parameters/
  center(); // center the servo
  pinMode(_LED_indicator, OUTPUT);  

}

void SensorMast::service() {
  getFlameReading();
  
  if (shouldMove)
    setServoSpin(); // rotate mast if we should
}

int SensorMast::getFlameReading() {
  flameVal = analogRead(_flamePin);
}

bool SensorMast::isFire() {
  return flameVal <= flameThreshold;
}


/*
  const int servoCenter = 90; // center position of servo
  int servoPos = servoCenter; // position of servo
  int servoDir = 0; // 0 is CCW, 1 is CW
  int maxCCW = 0;
  int maxCW = 180;
*/

void SensorMast::setServoSpin(int time) {
  if(servoDir == 0) // CW movement
  {
    if(servoPos <= maxCW) // time to go in other direction 
    {
      servoDir = 1;
      servoPos += servoStep;
    }
    else // haven't made it to limit, keep going CCW
      servoPos -= servoStep;
  }
  
  // CCW movement, servoDir = 1
  else {
    if (servoPos >= maxCCW)
    {
      servoDir = 0;
      servoPos -= servoStep;
    }
    else
      servoPos += servoStep;
  }
  servo.write(servoPos);
  delay(time);
}

void SensorMast::center() {
  servoPos = servoCenter;
  servo.write(servoPos);
}

int SensorMast::getServoAngle() {
  // make it into radians, with heading at pos. x-axis
//  int angle = -1*(servoPos - 90); // this is old // take pos, turn into +- angle from center, CCW pos, CW negative
//  return servoPos * (PI/180.0); // this is radians
  return servoPos; // angle in degrees
}


void SensorMast::indicateNear(){
  if (isFire())
  { 
  digitalWrite(_LED_indicator, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else {
  digitalWrite(_LED_indicator, LOW);    // turn the LED off by making the voltage LOW
  } 
}

void SensorMast::indicateWin(){
  digitalWrite(_LED_WIN, HIGH);   // turn the LED on (HIGH is the voltage level) Is called in the condition that the flame is extinguished, operates with no boolean
  }
  
void SensorMast::freeze() {
  shouldMove = false;
  servo.write(servoPos);
}

void SensorMast::unFreeze() {
  shouldMove = true;

}
