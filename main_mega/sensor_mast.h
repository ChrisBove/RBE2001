/**
 ********************************************************************************************************
 * @file    sensor_mast.h
 * @brief   sensor mast control class
 * @details Used to control the sensor mast servo and read flame sensor and ultrasonic sensor
 ********************************************************************************************************
 */

#ifndef sensor_mast_h
#define sensor_mast_h

#include "Arduino.h"

#include <Servo.h>  // import libraries


class SensorMast
{
  public:
   /**
   * @brief   Class constructor for sensor mast
   * @param   pins of servo motor, ultrasonic sensor, and flame sensor
   * @return  sensor mast object
   */
   SensorMast(int servoPin, int ultraPin, int flamePin);
   /**
   * @brief   attaches motors
   * @param   None
   * @return  None
   */
   void setupMast();
   
  private:
    int _servoPin, _ultraPin, _flamePin;
};

#endif
