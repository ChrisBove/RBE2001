/**
 ********************************************************************************************************
 * @file    cliff.cpp
 * @brief   cliff class
 * @details Includes data collected from line sensors, indicating cliff dangers
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "cliff.h"

CliffDetector::CliffDetector(int LeftLightPin, int RightLightPin){
  _LeftLightPin = LeftLightPin;      //Link the associated pins
  _RightLightPin = RightLightPin;
}

void CliffDetector::setHues(){  
  for(int x=0; x<50 ; x++){
    sum = analogRead(_LeftLightPin) + analogRead(_RightLightPin);
    delay(50);
  }
    int refWhiteHigh = sum/50;   //calibrated at power-up to set high limit of white range, verge of black
}


bool CliffDetector::falling(){
  return (analogRead(_LeftLightPin) >= refWhiteHigh || analogRead(_RightLightPin) >= refWhiteHigh); 
}


