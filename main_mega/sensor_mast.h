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
   * @brief   Runs all service routines for the sensor mast
   * @param   None
   * @return  None
   */
   void service();
   
   
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
   * @brief   returns reading from analog pin
   * @param   None
   * @return  int distance in inches
   */
   int getDistance();
   /**
   * @brief   grabs flame reading from sensor
   * @param   None
   * @return  int reading
   */
   int getFlameReading();
   /**
   * @brief   checks threshold to determine if there is a fire
   * @param   None
   * @return  true if there is a flame detected above threshold
   */
   bool isFire();
   
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
   * @return  heading in radians from x-axis: CCW is Positive
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
    
    const int flameThreshold = 850; // flame less than this
    
    const int servoCenter = 103; // center position of servo
    const int servoStep = 1;
    int servoPos = servoCenter; // position of servo
    int servoDir = 0; // 0 is CW, 1 is CCW
    int maxCW = 23;
    int maxCCW = 180;
};

#endif
