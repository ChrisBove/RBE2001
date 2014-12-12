/**
 ********************************************************************************************************
 * @file    virtual_bumper.h
 * @brief   virtual bumper class
 * @details takes range sensor data and makes it into a virtual bumper and steers the robot around obstacles
 ********************************************************************************************************
 */

#ifndef virtual_bumper_h
#define virtual_bumper_h

#include "Arduino.h"
#include "cliff.h"

class VirtualBumper {
  public:
   /**
   * @brief   Class constructor for virtual bumper
   * @param   None
   * @return  virtual bumper object
   */
   VirtualBumper();
   
   // defines directions of obstacles
   enum DIR { LEFT, CENTER, RIGHT, CENTER_L, CENTER_R, R_AND_L};
   enum SIDE {L, R};
   
   /**
   * @brief   Runs all service routines
   * @param   distance from ultra sonic, angle from ultrasonic in degrees
   * @return  None
   */
   void service(int distance, int angle);
   /**
   * @brief   returns the direction of an obstacle by checking the sensors
   * @param   None
   * @return  None
   */
   DIR obstacle();
   /**
   * @brief   returns true if an obstacle is found within a threshold for the IR sensor
   * @param   side of IR to check 
   * @return  true if obstacle found within threshold
   */
   bool seeObstacle(SIDE side);
   
   
  private:
   int _leftIR, _rightIR;
  
};

#endif
