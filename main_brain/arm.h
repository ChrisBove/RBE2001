/**
 ********************************************************************************************************
 * @file    arm.h
 * @brief   Arm control class
 * @details Used to control 4-bar linkage robot arm.
 ********************************************************************************************************
 */

#ifndef arm_h
#define arm_h

/*** INCLUDE FILES ***/
#include <Servo.h>
#include "button.h"

class Arm {
  
  public:
   /**
   * @brief   Class constructor for arm.
   * @param   motor pin and pot pin for arm
   * @return  Arm object
   */
    Arm(int motorPin, int potPin);
   /**
   * @brief   setups arm and attaches pins
   * @param   None
   * @return  None
   */
    void setupArm();
   /**
   * @brief   Uses PID control to drive arm to passed setpoint
   * @param   Setpoint in pot position, button object that limits arm movement
   * @return  None
   */
    void setPosition(int setPoint, Button& frontLimit);
    /**
   * @brief   checks if arm is in correct/desired position
   * @param   None
   * @return  true if in position, false if otherwise
   */
    bool isInPosition();
    /**
   * @brief   asks arm to go to the up position
   * @param   forward button limit
   * @return  true when at position for sequential runs
   */
    bool goUp(Button& frontLimit);
     /**
   * @brief   asks arm to go to the down position
   * @param   forward button limit
   * @return  true when at position for sequential runs
   */
    bool goDown(Button& frontLimit);
    
  private:
    /**
   * @brief   Drives motor in down direction
   * @param   output for motor, button to limit motion
   * @return  None
   */
    void rotateDown (float output, Button& frontLimit);
    /**
   * @brief   Drives motor in up direction
   * @param   output for motor, button to limit motion
   * @return  None
   */
    void rotateUp (float output, Button& frontLimit);
   /**
   * @brief   takes output from PID, and sends motor commands based on the output
   * @param   output from PID, button to limit motion
   * @return  None
   */
    void sendOutput(float output, Button& frontLimit);
    /**
   * @brief   scales PID output to use servo values
   * @param   output from PID
   * @return  float of output for motor
   */
    float scaleOutput (float output);
    
    int _motorPin; // pin that motor is attached to
    int _potPin;  // pin for pot
    
     // constants for PID, casually tuned for system
     const float Kp = 0.0001;
     const float Ki = 0.0000005; 
     const float Kd = 0.001;
     
     const int upPos = 696;    // pot positions
     const int downPos = 330;
     
     // ****** Variables ********
     float lastError = 0; // stored from previous value.
     float errorSum = 0;  // sum of errors
     int count = 0;  // used to reset sum
     int goodness = 0; // used to tell whether we are holding position
};

#endif
