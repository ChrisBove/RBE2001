/**
 ********************************************************************************************************
 * @file    virtual_bumper.cpp
 * @brief   virtual bumper class
 * @details takes range sensor data and makes it into a virtual bumper and steers the robot around obstacles
 ********************************************************************************************************
 */
 
 #include "Arduino.h"
 #include "virtual_bumper.h"
 
 VirtualBumper::VirtualBumper(int leftIR, int rightIR) {
   _leftIR = leftIR;
   _rightIR = rightIR;
 }
 
 
