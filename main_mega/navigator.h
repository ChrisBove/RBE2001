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
   /** @brief  checks to see if there is a flame and stops the servo
   * @param   None
   * @return  NONE
   */
   
   void checkFlame();
   
   // defines possible states of navigation
   enum Navigate { FIND_CANDLE, CALC_POSITION, EXTINGUISH, RETURN };
   
  private:
   
   Navigate state;
  
};

#endif
