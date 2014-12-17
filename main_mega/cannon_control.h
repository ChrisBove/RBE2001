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
  enum CannonS {LOCATE_FLAME, DRAW_BACK, GIVE_SLACK, TUG, AIM_AT_FLAME, SHOOT_CANNON};
  
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
   
   void giveTug();
   
   void AIM();
   
   void shootCannon();
   
   void service();
   
   void cannonOP();
   
   int giveAngle();
   
   void resetCannon();
   
   void slackComp();
   
   bool returnResult();
   
   private:
   
   CannonS state = LOCATE_FLAME;
   
   const int flameLimit = 100;
   
   int _gripperPin, _servoPin, _motorPin, _flamePin, _encoderPin1, _encoderPin2;
   int threshold = 500;
   int flameVal = 1000;
   int currentFlameVal;
   int currentFlamePos;
   int servoPosition;
   int servoMin = 50;
   int servoMax = 160;
   int counter = 0;
   int gripClosed = 0;
   int aimCount = 0;
   int cannonStartNum = 0;
   int num = 0;
   
   long oldPosition = 0;
   long newPosition;
   
   bool locateFlameTrue = true;
   bool drawBackTrue = false;
   bool drawBackCont = true;
   bool giveSlackTrue = false;
   bool giveTugTrue = false;
   bool shootCannonTrue = false;
   bool AIMTrue = false;
   bool cont = true;
   bool cannonStart = true;
   bool extinguished = false;
   
   bool flameFound = false;
};
#endif
