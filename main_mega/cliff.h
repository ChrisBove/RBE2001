/**
 ********************************************************************************************************
 * @file    cliff.h
 * @brief   cliff class
 * @details Includes data collected from line sensors, indicating cliff dangers
 ********************************************************************************************************
 */

#ifndef cliff_h
#define cliff_h

#include "Arduino.h"

class CliffDetector {
  public:
   /**
   * @brief   Constructor for cliff class
   * @param   (int LeftLightPin, int RightLightPin analog pins
   * @return  cliff object
   */
   CliffDetector(int LeftLightPin, int RightLightPin);
   /**
   * @brief   Calibrates white/black sensor values
   * @param   None
   * @return  LightSensor white range values (2 int)
   */
   int setHues();
   /**
   * @brief   checks threshold to determine if there is a cliff for R and L
   * @param   None
   * @return  true if a wheel is detected over the cliff
   */
   bool fallingR();
   bool fallingL();
   
  private:
    int _LeftLightPin, _RightLightPin;    
    int refWhiteHigh;   //calibrated at power-up to set high limit of white range, verge of black
    const int blackThresh = 230;   //Dead reckoned black-threshold value: 230, black appears past 315 in almost every scenario
    unsigned int sum; 
};

#endif
