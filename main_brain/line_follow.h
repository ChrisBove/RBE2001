#ifndef line_follow_h
#define line_follow_h

#include "drive_train.h"

class LineFollow {
  
  public:
    LineFollow(int leftPort, int middlePort, int rightPort, int backPort);
    
    
    void updateSensorData();
    void setThreshold(int value);
    int doLineFollowTillCross(DriveTrain& driving);
    int doLineFollow(DriveTrain& driving);
    int allOnCross();
    int makeBinNumber();
    
    void turnLeft();
    void turnRight();
    void halt();
    void forward();
    
    // number is the crossing to stop on
    int stopOnCrossing(DriveTrain& driving, int number);
    void resetCrossCount();
    
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
    
    int _white = 200; // white is less than
    
    int state = 2;
    
    int sumCrossings = 0;
    int lastRun = 0;
    LineSensors currentSensors;
    
};

#endif
