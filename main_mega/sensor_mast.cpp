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

SensorMast::SensorMast(int servoPin, int ultraPin, int flamePin) {
  _servoPin = servoPin;
  _ultraPin = ultraPin;
  _flamePin = flamePin;
}

void SensorMast::setupMast() {
  servo.attach(_servoPin);
  
}
