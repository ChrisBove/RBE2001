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

int CliffDetector::setHues(){        //This function was developed, but never implemented 
   refWhiteHigh = 0; 
  for(int x=1; x<=50 ; x++){
    sum = analogRead(_LeftLightPin) + analogRead(_RightLightPin);
    //delay(5);
  }
     refWhiteHigh = (sum/100)+150;   //calibrated at power-up to set high limit of white range, verge of black
     }

bool CliffDetector::fallingR(){
  int temp_A = analogRead(_RightLightPin);
  return (temp_A >= blackThresh); 
}

bool CliffDetector::fallingL(){
  int temp_B = analogRead(_LeftLightPin);
  return (temp_B >= blackThresh); 
}

