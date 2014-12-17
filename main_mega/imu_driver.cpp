/**
 ********************************************************************************************************
 * @file    imu_driver.cpp
 * @brief   imu class
 * @details Handles the IMU functions
 ********************************************************************************************************
 */
 
#include "imu_driver.h"
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;

IMUDriver::IMUDriver() {
   
}
 
void IMUDriver::setupIMU() {
  Wire.begin();
  compass.init();
  compass.enableDefault();
}

void IMUDriver::service() {
  compass.read();
}

bool IMUDriver::isTipped() {
  return (abs(compass.a.z) < abs(compass.a.y)) || (abs(compass.a.z) < abs(compass.a.x));
}
