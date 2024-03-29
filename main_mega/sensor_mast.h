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
   * @param   pins of servo motor, flame sensor, and LED's
   * @return  sensor mast object
   */
   SensorMast(int servoPin, int flamePin, int LED_indicator, int LED_WIN);
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
   void setServoSpin(int time = 15);
   /**
   * @brief   keeps servo at current location
   * @param   None
   * @return  None
   */
   void freeze();
   /**
   * @brief   re-enables servo rotation
   * @param   None
   * @return  None
   */
   void unFreeze();
   /**
   * @brief   calculates the angle the servo is at based on last command
   * @param   None
   * @return  heading in degrees from x-axis: CCW is Positive, 90 is forward
   */
   int getServoAngle();
   /**
   * @brief   centers servo
   * @param   None
   * @return  None
   */
   void center();
   /**
   * @brief   makes an LED light up when the robot is in proximity of the candle (the mast flame vantage-point)
   * @param   None--calls the IR reading
   * @return  None--performs an action
   */
   void indicateNear();
/**
   * @brief   makes an LED light up when the candle has been extinguished (will only be called during the CANNON STATE)
   * @param   None--calls the IR reading
   * @return  None--performs an action
   */
   void indicateWin();
   /**
   * @brief   returns the calibrated center of the servo
   * @param   None
   * @return  heading in degrees from x-axis: CCW is Positive, 90 is forward
   */
   int getServoCenter();
   
  private:
    int _servoPin, _flamePin, _LED_indicator, _LED_WIN; // pins that are connected
    int flameVal; // stores value for flame sensor
    
    const int flameThreshold = 600; // flame less than this
    
    const int servoCenter = 110; // center position of servo
    const int servoStep = 1;
    int servoPos = servoCenter; // position of servo
    int servoDir = 0; // 0 is CW, 1 is CCW
    int maxCW = 23; // max CW position, about 90 degree angle to front of robot
    int maxCCW = 180; // max CCW position
    
    bool shouldMove = true; // controls if the mast should move
};

#endif
