/**
 ********************************************************************************************************
 * @file    cannon_control.h
 * @brief   cannon control class
 * @details Used to control cannon
 ********************************************************************************************************
 */

#ifndef cannon_control_h
#define cannon_control_h

#include "Arduino.h"

#include <Servo.h>  // import libraries
#include <Encoder.h>


class CannonControl
{
  public:
  //enum cannonState {WAIT_FOR_INSTRUCT, FIND_FLAME, DRAW_BACK, GIVE_SLACK, AIM_AT_FLAME, SHOOT_CANNON, CHECK_FLAME};
  
  CannonControl(int gripperPin, int servoPin, int motorPin, int flamePin);
     /**
   * @brief   attaches motors, servos, and sensors
   * @param   None
   * @return  None
   */
   void setupCannon();
   
   void checkFlame();
   
   void locateFlame();
   
   void drawBack();
   
   void giveSlack();
   
   void AIM();
   
   void shootCannon();
   
   private:
   
   Servo winch;
   Servo hinge;
   Servo grip;
   
   const int flameLimit = 100;
   
   int _gripperPin, _servoPin, _motorPin, _flamePin, _encoderPin1, _encoderPin2;
   int flameVal = 1000;
   int currentFlameVal;
   int currentFlamePos;
   int servoPos;
   int servoMin = 40;
   int servoMax = 100;
   int counter = 0;
   
   long oldPosition = 0;
   long newPosition;
   
   bool flameFound = false;
};
#endif
