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
     /**
   * @brief   initializes the cannon
   * @param   None
   * @return  None
   */
   void checkFlame();
     /**
   * @brief   Checks the current value of the flame sensor and records the current position of the servo for the lowest flame reading
   * @param   None
   * @return  None
   */
   void locateFlame();
     /**
   * @brief   sweeps the Hinge servo from the min value to the max value, and calls checkFlame() repeatedly, as it scans for the flame
   * @param   None
   * @return  None
   */
   void drawBack();
     /**
   * @brief   winds the winch to a sertain position then closes the grip
   * @param   None
   * @return  None
   */
   void giveSlack();
     /**
   * @brief   unwinds the winch
   * @param   None
   * @return  None
   */
   void giveTug();
     /**
   * @brief   rotates the hinge servo to its min position
   * @param   None
   * @return  None
   */
   void AIM();
     /**
   * @brief   rotates the hinge servo to the position recorded by checkFlame()
   * @param   None
   * @return  None
   */
   void shootCannon();
     /**
   * @brief   opens the gripper
   * @param   None
   * @return  None
   */
   void service();
     /**
   * @brief   runs the entire process for the cannon locateFlame=>drawBack=>giveSlack=>giveTug=>shootCannon=>
   *          this secion of code was not implemented in the final design
   * @param   None
   * @return  None
   */
   void cannonOP();
     /**
   * @brief   runs the entire process for the cannon locateFlame=>drawBack=>giveSlack=>giveTug=>shootCannon=>
   * @param   None
   * @return  None
   */
   int giveAngle();
     /**
   * @brief   calculates the angle of the cannon when it found the flame
   * @param   None
   * @return  The angle of the cannon
   */
   void resetCannon();
     /**
   * @brief   resets the cannon's variables
   * @param   None
   * @return  None
   */
   void slackComp();
     /**
   * @brief   moves the winch motor a few degrees to adjust for error in the encoder
   *          this section of code was not implemented in the final design
   * @param   None
   * @return  None
   */
   bool returnResult();
     /**
   * @brief   returns wether the flame has been extingushed or not
   * @param   None
   * @return  true - is extingushed, false - is not extinguished
   */
   private:
   
   CannonS state = LOCATE_FLAME; // sets the starting state for cannonOP

   const int flameLimit = 100; // 
   
   int _gripperPin, _servoPin, _motorPin, _flamePin, _encoderPin1, _encoderPin2; // pins that are connected
   int threshold = 500; // sets the threshold for the flame sensor
   int flameVal = 1000; // starting value for the recorded flame value
   int currentFlameVal; // the current value for the recorded flame value
   int currentFlamePos; // the current position of the servo
   int servoPosition;   // the recored position of the servo at the lowest flame reading
   int servoMin = 50;   // sets the minnimum value of the servo position
   int servoMax = 160;  // sets the maximum value of the servo position
   int counter = 0;     // counter used in operation of the hinge servo, for adjusting the amount of time it operates
   int gripClosed = 0;  // counter used in operation of the gripper
   int aimCount = 0;    // counter used in operation of the hinge while aiming
   int cannonStartNum = 0;  // was not implemented
   int num = 0;         // counter used in give slackComp()
   
   long oldPosition = 0; // the recorded old position used in opertation of the hinge
   long newPosition;     // the new recorded position used in operation of the hinge
   
   bool locateFlameTrue = true;
   bool drawBackTrue = false;
   bool drawBackCont = true;   // says to continue the operation of drawBack()
   bool giveSlackTrue = false;
   bool giveTugTrue = false;
   bool shootCannonTrue = false;
   bool AIMTrue = false;
   bool cont = true;
   bool cannonStart = true;
   bool extinguished = false; // says if the candle has been extinguished
   
   bool flameFound = false;  // says that the cannon has not found the candle yet
};
#endif
