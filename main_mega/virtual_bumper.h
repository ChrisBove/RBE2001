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
   * @param   pin of analog ultrasonic sensor, digital pin of ultrasonic
   * @return  virtual bumper object
   */
   VirtualBumper(int ultraPin, int digUltraPin);
   
   // defines directions of obstacles
   enum DIR { LEFT, CENTER, RIGHT, CENTER_L, CENTER_R, R_AND_L};
   enum SIDE {L, R};
   
   /**
   * @brief   Attaches pins required. Should be called in main setup
   * @param   None
   * @return  None
   */
   void setupBumper();
   
   /**
   * @brief   Runs all service routines, reading sensors
   * @param   None
   * @return  None
   */
   void service();
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
   
   // Ultrasonic functions
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
   
   
  private:
   int _leftIR, _rightIR, _ultraPin, _digUltraPin;
   
   int reading; // stores distance in inches
   int distance; // stores distance from ultrasonic in cm
  
};

#endif
