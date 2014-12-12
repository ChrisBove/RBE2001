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

class VirtualBumper {
  public:
   /**
   * @brief   Class constructor for virtual bumper
   * @param   left IR pin, right IR pin
   * @return  virtual bumper object
   */
   VirtualBumper(int leftIR, int rightIR);
   /**
   * @brief   Runs all service routines
   * @param   None
   * @return  None
   */
   void service();
   
   
  private:
   
  
};

#endif
