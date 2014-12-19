/**
 ********************************************************************************************************
 * @file    lcd.h
 * @brief   lcd class
 * @details Controls LCD with certain parameters
 ********************************************************************************************************
 */

#ifndef lcd_h
#define lcd_h

#include "Arduino.h"

class LCD {
  public:
   /**
   * @brief   Class constructor for sonic assembler
   * @param   none
   * @return  sonic assembler object
   */
   LCD();
   /**
   * @brief   Runs all service routines, prints position if it is updated
   * @param   None
   * @return  None
   */
   void service();
   /**
   * @brief   sets up the lcd
   * @param   None
   * @return  None
   */
   void setupLCD();
   /**
   * @brief   takes in the position and immediately prints it
   * @param   float x coordinates, float y coordinates, and float z coordinates
   * @return  None
   */
   void printLocationNow(float x, float y, float z);
   /**
   * @brief   takes in the position and saves it
   * @param   float x coordinates, float y coordinates, and float z coordinates
   * @return  None
   */
   void updateLocation(float x, float y, float z);
   /**
   * @brief   prints current location
   * @param   None
   * @return  None
   */
   void refresh();
   
  private:
   bool shouldRefresh = true; // flags if the lcd should refresh during services
   
   float xPos = 0; // store location of robot
   float yPos = 0;
   float zPos = 0;
  
};

#endif
