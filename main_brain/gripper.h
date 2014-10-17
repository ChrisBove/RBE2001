/**
 ********************************************************************************************************
 * @file    gripper.h
 * @brief   gripper control class
 * @details controls the gripper (rack and the hand/claw)
 ********************************************************************************************************
 */

#ifndef gripper_h
#define gripper_h

#include "Arduino.h"

#include <Servo.h>

class Gripper {
  public:
  // enum for managing gripper state
  enum gripperState {WAIT_FOR_INSTRUCT, INIT_CLOSE_GRIP, CLOSE_GRIP, INIT_RETRACT, RETRACT, INIT_EXTEND, EXTEND, INIT_EXTENDLIM, EXTENDLIM, INIT_OPEN_GRIP, OPEN_GRIP};
  Gripper(gripperState state); // holds current gripper state
    // states for all the gripper movements
    gripperState grippyStateClose = INIT_CLOSE_GRIP;
    gripperState grippyStateRetract = INIT_RETRACT;
    gripperState grippyStateExtend = INIT_EXTEND;
    gripperState grippyStateExtendLim = INIT_EXTENDLIM;
    gripperState grippyStateOpen = INIT_OPEN_GRIP;
    
    /**
   * @brief   Class constructor for gripper
   * @param   pins of motors: claw and rack position servos
   * @return  Gripper object
   */
    Gripper(int gripServo, int rackServo);
   /**
   * @brief   attaches gripper servos
   * @param   None
   * @return  None
   */
    void attachMotors();
   /**
   * @brief   closes the grip
   * @param   None
   * @return  true when grip is closed
   */
    bool closeTheGrip();
    /**
   * @brief   retracts the gripper using the rack
   * @param   None
   * @return  true when gripper is retracted (up/in)
   */
    bool retractTheGrip();
   /**
   * @brief   extends the grip
   * @param   None
   * @return  true when grip is extended (down/out)
   */
    bool extendTheGrip();
   /**
   * @brief   extends the grip, but at a shorter distance
   * @param   None
   * @return  true when grip is extended
   */
    bool extendLimTheGrip();
   /**
   * @brief   opens the grip
   * @param   None
   * @return  true when grip is opened
   */
    bool openTheGrip();

    
  private:
   /**
   * @brief   extends the grip for time
   * @param   None
   * @return  true when grip is extended after time
   */
    bool extend();
   /**
   * @brief   extends the grip to a smaller position for a time
   * @param   None
   * @return  true when grip is extended to a limited amount
   */
    bool extendLim();
   /**
   * @brief   retracts the grip for a time
   * @param   None
   * @return  true when retraction time has passed
   */
    bool retract();
   /**
   * @brief   opens the grip for a time
   * @param   None
   * @return  true when time has passed
   */
    bool openGrip();
   /**
   * @brief   closes the grip for a time
   * @param   None
   * @return  true when time has passed
   */
    bool closeGrip();
   /**
   * @brief   sets the start time
   * @param   None
   * @return  None
   */
    void setReactTime();
    
    // used servos
    Servo rack;
    Servo grip;
    
    int _gripServo;
    int _rackServo;
    
    int reactTime = 0; // initialize time to 0
};

#endif
