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

SensorMast::SensorMast(int servoPin, int ultraPin, int flamePin, int digUltraPin) {
  _servoPin = servoPin;
  _ultraPin = ultraPin;
  _flamePin = flamePin;
  _digUltraPin = digUltraPin;
}

void SensorMast::setupMast() {
  servo.attach(_servoPin);
  pinMode(_digUltraPin, INPUT);
}

int SensorMast::getAnalogDistance() {
  reading = 1+analogRead(_ultraPin)/2; // divide by 2: 4.9mV/2cm
  return reading;
}

int SensorMast::getDigitalDistance() {
  distance = pulseIn(_digUltraPin, HIGH)/58; // 
  return distance;
}

int SensorMast::getFlameReading() {
  flameVal = analogRead(_flamePin);
}

/*
  const int servoCenter = 90; // center position of servo
  int servoPos = servoCenter; // position of servo
  int servoDir = 0; // 0 is CCW, 1 is CW
  int maxCCW = 0;
  int maxCW = 180;
*/

void SensorMast::setServoSpin(int time) {
  if(servoDir == 0) // CCW movement
  {
    if(servoPos <= 0) // time to go in other direction 
    {
      servoDir = 1;
      servoPos++;
    }
    else // haven't made it to limit, keep going CCW
      servoPos --;
  }
  
  // CW movement, servoDir = 1
  else {
    if (servoPos >= 180)
    {
      servoDir = 0;
      servoPos --;
    }
    else
      servoPos ++;
  }
  servo.write(servoPos);
  delay(time);
}
  
