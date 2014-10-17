/**
 ********************************************************************************************************
 * @file    line_follow.h
 * @brief    control class
 * @details controls the gripper (rack and the hand/claw)
 ********************************************************************************************************
 */

#ifndef line_follow_h
#define line_follow_h

#include "drive_train.h"

class LineFollow {
  
  public:
   /**
   * @brief   Class constructor for line follow
   * @param   pins of the analog line sensors
   * @return  LineFollow object
   */
    LineFollow(int leftPort, int middlePort, int rightPort, int backPort);
   /**
   * @brief   calibrates line sensor threshold. Robot must be centered over line.
   * @param   None
   * @return  None
   */
    void calibrate();
   /**
   * @brief   reads analog values over the ADC and stores digital results in the class.
   * @param   None
   * @return  None
   */
    void updateSensorData();
   /**
   * @brief   sets the threshold to the passed value
   * @param   int 0-1023, values under this are "white/false", values over are "black/true"
   * @return  None
   */
    void setThreshold(int value);
   /**
   * @brief   executes doLineFollow until a cross is hit, then the robot stops
   * @param   DriveTrain to control, direction of robot travel.
   * @return  1 if we've met the cross, 0 if cross has not been reached.
   */
    int doLineFollowTillCross(DriveTrain& driving, DriveTrain::dirTravel dir);
   /**
   * @brief   Does line following, does not stop at crosses 
   * @param   Drivetrain object to control, direction of travel
   * @return  1 if all sensors are on the cross.
   */
    int doLineFollow(DriveTrain& driving, DriveTrain::dirTravel dir);
   /**
   * @brief   checks if all the sensors are on the cross
   * @param   None
   * @return  1 if all sensors read true/black
   */
    int allOnCross();
   /**
   * @brief   linefollows then stops on the specified crossing number
   * @param   Drivetrain object to control, the number of the crossing to stop on, direction of travel
   * @return  1 if the crossing has been reached
   */
    int stopOnCrossing(DriveTrain& driving, int number, DriveTrain::dirTravel dir);
   /**
   * @brief   resets the internal crossing count, needed after asking robot to reach a crossing
   * @param   None
   * @return  None
   */
    void resetCrossCount();
   /**
   * @brief   sets an internal flag to say whether robot should keep moving on crosses
   * @param   true if we desire the robot to keep moving when all sensors are on a cross
   * @return  None
   */
    void setForwardOnCross(bool goesForward);
    
    // storage of line sensor values
    struct LineSensors {
      bool left;
      bool middle;
      bool right;
      bool back;
    };
    
  private:
  
    // VARIABLES
    int _leftPort; 
    int _middlePort; 
    int _rightPort; 
    int _backPort;
    
    int _whiteThreshold = 200; // white is less than this, initialize to experi. value
    
    int state = 2; // defined for switch statement, stores last line follow state
    
    int sumCrossings = 0; // sum of the crossings we've hit
    int lastRunValue = 0;      // 1 if the last run value was on the line
    LineSensors currentSensors; // stores current sensor array
    
    bool goForwardOnCross = false; // flag for cross behavior
    
};

#endif
