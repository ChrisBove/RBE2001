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
#include "vfh.h"

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
   
   // defines possible states of navigation
   enum Navigate { FIND_CANDLE, CALC_POSITION, EXTINGUISH, RETURN };
   
  private:
   
   Navigate state; // state to keep
   
   // global pointers
//   VFH::grid * myGrid;
//   VFH::hist * myHist;
   
//   VFH::grid theGrid; // grid for VFH
//   VFH::hist theHist; // histogram for VFH
   
   int lastServoPos = 90;
   int measureCount = 0; 
  
};

#endif
