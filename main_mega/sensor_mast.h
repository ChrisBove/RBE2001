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
   SensorMast(int servoPin, int ultraPin, int flamePin, int digUltraPin);
   /**
   * @brief   attaches motors
   * @param   None
   * @return  None
   */
   void setupMast();
   /**
   * @brief   grabs distance from ultrasonic analog port
   * @param   None
   * @return  int distance in inches
   */
   int getAnalogDistance();
   /**
   * @brief   grabs distance from ultrasonic PW
   * @param   None
   * @return  int distance in cm
   */
   int getDigitalDistance();
   /**
   * @brief   grabs flame reading from sensor
   * @param   None
   * @return  int reading
   */
   int getFlameReading();
   
   
  private:
    int _servoPin, _ultraPin, _flamePin, _digUltraPin;
    int reading; // stores distance in inches
    int distance; // stores distance from ultrasonic in cm
    int flameVal; // stores value for flame sensor
};

#endif
