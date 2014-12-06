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
  distance = pulseIn(_digUltraPin, HIGH)/147; // 
  return distance;
}

