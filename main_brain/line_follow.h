#ifndef line_follow_h
#define line_follow_h

#include "drive_train.h"

class LineFollow {
  
  public:
    LineFollow(int leftPort, int middlePort, int rightPort, int backPort);
    void calibrate();
    
    void updateSensorData();
    void setThreshold(int value);
    int doLineFollowTillCross(DriveTrain& driving, int dir);
    int doLineFollow(DriveTrain& driving, int dir);
    int allOnCross();
    int makeBinNumber();
    
    // number is the crossing to stop on
    int stopOnCrossing(DriveTrain& driving, int number, int dir);
    void resetCrossCount();
    void setForwardOnCross(bool goesForward);
    
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
    
    int lSpeed;
    int rSpeed;
    
    int _whiteThreshold = 200; // white is less than this
    
    int state = 2;
    
    int sumCrossings = 0;
    int lastRun = 0;
    LineSensors currentSensors;
    
    bool goForwardOnCross = false;
    
};

#endif
