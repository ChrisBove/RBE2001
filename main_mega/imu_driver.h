/**
 ********************************************************************************************************
 * @file    imu_driver.h
 * @brief   imu class
 * @details Handles the IMU functions
 ********************************************************************************************************
 */

#ifndef imu_driver_h
#define imu_driver_h

#include "Arduino.h"
#include <LSM303.h>
// if you can't find this library, install it here: https://github.com/pololu/lsm303-arduino

class IMUDriver {
  public:
   /**
   * @brief   Class constructor for IMU Driver
   * @param   none
   * @return  IMU Driver object
   */
   IMUDriver();
   /**
   * @brief   sets up the IMU
   * @param   None
   * @return  None
   */
   void setupIMU();
   /**
   * @brief   Runs all service routines
   * @param   None
   * @return  None
   */
   void service();
   /**
   * @brief   checks if the robot is getting tipped (z accels less than x or y)
   * @param   None
   * @return  True if being tipped
   */
   bool isTipped();
   
  private:
    
  
};

#endif
