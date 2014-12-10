/**
 ********************************************************************************************************
 * @file    drive_train.h
 * @brief   drive train control class
 * @details Used to control drive train
 ********************************************************************************************************
 */

#ifndef drive_train_h
#define drive_train_h

#include "Arduino.h"

#include <Servo.h>  // import libraries
#include <Encoder.h>

class DriveTrain
{
  public:
  enum dirTravel {FORWARD, BACKWARD}; // directions of travel
     /**
   * @brief   Class constructor for drive train
   * @param   pins of motors, whether they are inverted
   * @return  drive train object
   */
    DriveTrain(int leftPin, int rightPin, bool leftInverted, bool rightInverted);
     /**
   * @brief   attaches motors
   * @param   None
   * @return  None
   */
    void setupDriveTrain();
    /**
   * @brief   services drivetrain functionality
   * @param   None
   * @return  None
   */
    void service();
         /**
   * @brief   moves motors if they are enabled
   * @param   motor values
   * @return  None
   */
    void moveMotors(int leftVal, int rightVal);
    /**
   * @brief   drives robot in given direction
   * @param   -90 (left) to 90 (right)
   * @return  None
   */
    void moveInDir(int dir);
    
     /**
   * @brief   turns left, either forward or reverse
   * @param   direction of travel
   * @return  None
   */
    void turnLeft(dirTravel dir);
         /**
   * @brief   turns right
   * @param   direction of travel
   * @return  None
   */
    void turnRight(dirTravel dir);
         /**
   * @brief   sharp turn to the left
   * @param   dir. of travel
   * @return  None
   */
    void sharpTurnLeft(dirTravel dir);
         /**
   * @brief   sharp turn to right
   * @param   dir. of travel
   * @return  None
   */
    void sharpTurnRight(dirTravel dir);
         /**
   * @brief   stops motors
   * @param   none
   * @return  None
   */
    void halt();
       /**
   * @brief   moves forward
   * @param   None
   * @return  None
   */
    void forward();
         /**
   * @brief   goes in reverse
   * @param   None
   * @return  None
   */
    void reverse();
         /**
   * @brief   drives motors (turns) with passed values
   * @param   left turning value, right turning value
   * @return  None
   */
    void turn(int lval, int rval);
         /**
   * @brief   turns 45 degrees on time estimation
   * @param   true to go right, false to go left
   * @return  true when turn has completed
   */
    bool turn45(bool isRight);
   /**
   * @brief   does a 180 based on timing
   * @param   true to go right, false to go left
   * @return  true when turn has completed
   */
    bool turn180(bool isRight);
   /**
   * @brief   sets the clock used for turning time
   * @param   None
   * @return  none
   */
    void setTime();
    /**
   * @brief   backs up using case statements
   * @param   None
   * @return  1 when backup completed
   */
    int backupABit();
  /**
   * @brief   backups for a specified time
   * @param   None
   * @return  true if time has elapsed
   */
    bool backupForTime();
  /**
   * @brief   drives forwards for a time using cases
   * @param   None
   * @return  1 when movement completed
   */
    int forwardABit();
   /**
   * @brief   turns around
   * @param   true to turn right, false for left
   * @return  true when turn is completed
   */
    bool turnAround(bool isRight);
    
    // enums for switch states
    enum ReverseState {INIT_BACKUP, BACKUP};
    enum ForwardState {INIT_FORWARD, RUN_FORWARD};
    enum turnAroundState {TURN_OFF_LINE, TURN_TILL_LINE};
    // variables to hold state
    turnAroundState turnState = TURN_OFF_LINE;
    ReverseState revState = INIT_BACKUP;
    ForwardState forwardState = INIT_FORWARD;
    
    struct my_position
    {
      float x;        /* inch */
      float y;        /* inch */
      float theta;    /* radian (clockwise from y-axis) */
    };
    
  private:
  
    bool shouldMove = true; // flag if motors should move
    
   /**
   * @brief   goes froward for a particular time
   * @param   None
   * @return  true when movement complete
   */
    bool forwardForTime();
  
    // VARIABLES
    Servo left;       // Declare drive motors
    Servo right;
    int _leftPin;
    int _rightPin;
    
    //inititialize the motor drive constants to "stopped"
    int _leftDrive = 0;  
    int _rightDrive = 0;
    
    int leftStop = 0;
    int rightStop = 0;
    
    int leftInversion = 1;
    int rightInversion = 1;
    
    int roamSpeed = 45; // how fast the robot roams at for VFH
//    #define radius 1.375    // radius of drive wheels
//    #define distance 9.63    // separation between wheels
    
    int startTime = 0;    // stores the start time to calculate elapsed time
    
    // encoder things:
    bool update_pos = true;
    struct my_position current_position; // position used by the thread
    struct my_position transformed;  // transformed into our X-Y coordinate frame
    
    /**
   * @brief   resets position
   * @param   None
   * @return  None
   */
    void initialize_odometry();
    /**
   * @brief   runs the code that calculates the odometry from the encoders
   * @param   None
   * @return  None
   */
    void odometer_thread();
   /**
   * @brief   transforms the coordinate system
   * @param   None
   * @return  None
   */
    void transform();
    
    
    
};

#endif
