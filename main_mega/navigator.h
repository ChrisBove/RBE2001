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
   /** @brief  checks to see if there is a flame and centers the robut to the flame
   * @param   None
   * @return  if ended or not
   */
   
   bool centerFlame();
   /** @brief  goes to the flame
   * @param   None
   * @return  if ended or not
   */
   bool goToFlame();
   /**
   * @brief   makes decision on what the next robot action should be
   * @param   None
   * @return  None
   */
   void chooseAction();
   /**
   * @brief   Services VFH
   * @param   None
   * @return  None
   */
   void doVFH();
   /**
   * @brief   Services bumper
   * @param   None
   * @return  None
   */
   void doBumper();
   
   // defines possible states of navigation
   enum Navigate { LOCATE_CANDLE, SPIN_TO_CANDLE, GET_CLOSE_TO_CANDLE, CALC_POSITION, EXTINGUISH, RETURN, TEST};
   
   /**
   * @brief   combines robot position and IR sensor ranges to provide X, Y, Z, theta coordinates of the candle
   * @param   No? 
   * @return  None: sends LCD output
   */
   void candle_Position(); 
 
 
  private:
   
   Navigate state = LOCATE_CANDLE; // state to keep
   
   // global pointers
//   VFH::grid * myGrid;
//   VFH::hist * myHist;
   
//   VFH::grid theGrid; // grid for VFH
//   VFH::hist theHist; // histogram for VFH
   
   int lastServoPos = 90;
   int measureCount = 0;
   bool isFirstTime = true;
   bool haveSeenNoFire = false;
  
};

#endif
