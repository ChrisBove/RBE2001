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
#include "drive_train.h"

class VirtualBumper {
  public:
   /**
   * @brief   Class constructor for virtual bumper
   * @param   pin of analog ultrasonic sensor, digital pin of ultrasonic
   * @return  virtual bumper object
   */
   VirtualBumper(int ultraPin, int digUltraPin);
   
   // defines directions of obstacles
   enum DIR { LEFT, CENTER, RIGHT, CENTER_L, CENTER_R, L_AND_R, NONE, ALL};
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
   * @brief   returns the direction of a cliff obstacle by checking the cliff sensors
   * @param   None
   * @return  None
   */
   DIR cliffObstacle();
   /**
   * @brief   returns true if an obstacle is found within a threshold for the IR sensor
   * @param   side of IR to check 
   * @return  true if obstacle found within threshold
   */
   bool seeObstacleIR(SIDE side);
   /**
   * @brief   returns true if an obstacle is sensed within a threshold for the ultrasonic sensor
   * @param   None
   * @return  true if obstacle found within threshold
   */
   bool seeObstacleUS();
   
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
   
   // drivetrain control
   /**
   * @brief   Controls the driveTrain to avoid obstacles
   * @param   None
   * @return  None
   */
   void steerMe(DriveTrain& drive);
   /**
   * @brief   Controls the driveTrain to follow walls
   * @param   None
   * @return  None
   */
   void wallFollow(DriveTrain& drive);
   
   
  private:
   int _leftIR, _rightIR, _ultraPin, _digUltraPin;
   
   int reading; // stores distance in inches
   int distance; // stores distance from ultrasonic in cm
   
   // 2.3V for 10cm object, 2.75V for 8cm object, 3.15V for 6cm and below 6 cm obstacles voltage again decreases
   const int sharpThreshold = 563; // object if greater than this threshold at 8cm
   
   const int ultraThreshold = 5; // object if less than 5 inches
   
   DIR cliffDir; // stores last direction of cliff obstacle
   DIR bumperDir; // stores last direction of bumper obstacle
   
   bool stillOnCliff = false; // remains true until a cliff event has fully past
   bool stillBumped = false; // remains true until a bumper event has fully past
   
   bool backupDone = false; // true if backup has completed and a reversal can begin
};

#endif
