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
   
   // *** Servo functions ****
   /**
   * @brief   spins the servo
   * @param   int delay in ms between each step
   * @return  None
   */
   void setServoSpin(int time = 20);
   /**
   * @brief   keeps servo at current location
   * @param   None
   * @return  None
   */
   void freeze();
   /**
   * @brief   calculates the angle the servo is at based on last command
   * @param   None
   * @return  -90 to 90 degrees for heading
   */
   int getServoAngle();
   /**
   * @brief   centers servo
   * @param   None
   * @return  None
   */
   void center();
   
  private:
    int _servoPin, _ultraPin, _flamePin, _digUltraPin;
    int reading; // stores distance in inches
    int distance; // stores distance from ultrasonic in cm
    int flameVal; // stores value for flame sensor
    
    const int servoCenter = 103; // center position of servo
    const int servoStep = 1;
    int servoPos = servoCenter; // position of servo
    int servoDir = 0; // 0 is CW, 1 is CCW
    int maxCCW = 0;
    int maxCW = 180;
};

#endif
