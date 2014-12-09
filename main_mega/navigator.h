/**
 ********************************************************************************************************
 * @file    navigator.h
 * @brief   navigator class
 * @details controls the robot using sensors and the drivetrain
 ********************************************************************************************************
 */

#ifndef navigator_h
#define navigator_h

#include "Arduino.h"

class Navigator {
  public:
   /**
   * @brief   Class constructor for navigator
   * @param   none
   * @return  navigator object
   */
   Navigator();
   /**
   * @brief   Runs startup routines
   * @param   None
   * @return  None
   */
   void setupNavigator();
   /**
   * @brief   Runs all service routines
   * @param   None
   * @return  None
   */
   void service();
   
   
  private:
   
   
  
};

#endif
