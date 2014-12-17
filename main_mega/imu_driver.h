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
   * @brief   Runs all service routines
   * @param   None
   * @return  None
   */
   void service();
   
  private:
    
  
};

#endif
