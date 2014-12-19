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
    
    // from odometer.c - Copyright (C) 2000, Dafydd Walters
    struct my_position
    {
      float x;        /* inch */
      float y;        /* inch */
      float theta;    /* radian (clockwise from y-axis) */
    };
    
    
    /**
   * @brief   gets x coordinate of robot position
   * @param   None
   * @return  float of x coordinate (+ axis to right of robot)
   */
    float getX();
    /**
   * @brief   gets y coordinate of robot position
   * @param   None
   * @return  float of y coordinate (+ axis to front of robot)
   */
    float getY();
    /**
   * @brief   gets heading of robot position
   * @param   None
   * @return  float of heading: radians from X-axis (CCW is positive)
   */
    float getHeading();
    /**
   * @brief   gets heading of robot position with the unbounded theta
   * @param   None
   * @return  float of heading: radians from X-axis (CCW is positive)
   */
    float getUnboundedHeading();
    /**
   * @brief   gets heading of robot position
   * @param   None
   * @return  int of heading: Deg from X-axis (CCW is positive)
   */
    int getHeadingDeg();
    
     
   /**
   * @brief   commands the robot to backup a specified number of inches, controlled by _____ sensor readings
   * @param   number of inches desired to reverse
   * @return  Boolean indicating if the translation has been accomplished
   */     
   bool backupX(float inches);
   
   /**
   * @brief   commands the robot to rotate a specified number of radians, controlled by heading IMU readings
   * @param   number of radians desired to spin
   * @return  Boolean indicating if the rotation has been accomplished
   */
   bool rotateX(float spin);
   
    
    
  private:
  
    bool shouldMove = true; // flag if motors should move
  
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
    
    int roamSpeed = 20; // how fast the robot roams at for VFH
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
   * @brief   transforms the coordinate system to that used by our robot: y is front, x is right, theta is from x axis - CCW is positive
   * @param   None
   * @return  None
   */
    void transform();    
   
   
   bool new_command = true; //This flags whether the position-controlled functions are operating under initial commands
   float target = 0; // target theta to turn
   
   bool new_pos_command = true; // flag for new data
   float pos_target_x = 0; // target positions for reversals
   float pos_target_y = 0;
   const float pos_error = 2; // acceptable error for reversals
   
   float unboundedTheta = 0; // theta that is not bounded between 0 and PI
};

#endif
