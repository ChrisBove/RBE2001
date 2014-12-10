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

int CliffDetector::setHues(){  
   refWhiteHigh = 0; 
  for(int x=1; x<=50 ; x++){
    sum = analogRead(_LeftLightPin) + analogRead(_RightLightPin);
    //delay(5);
  }
     refWhiteHigh = (sum/100)+150;   //calibrated at power-up to set high limit of white range, verge of black
     }

int blackThresh = 230; 

bool CliffDetector::fallingR(){
  return (analogRead(_RightLightPin) >= blackThresh); 
}

bool CliffDetector::fallingL(){
  return (analogRead(_LeftLightPin) >= blackThresh); 
}

