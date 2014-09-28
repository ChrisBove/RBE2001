#ifndef line_follow_h
#define line_follow_h

#include "drive_train.h"

class LineFollow {
  
  public:
    LineFollow(int leftPort, int middlePort, int rightPort, int backPort);
    
    struct LineSensors {
      bool left;
      bool middle;
      bool right;
      bool back;
    };
    
    void updateSensorData();
    void setThreshold(int value);
    int doLineFollowTillCross(DriveTrain& driving);
    int allOnCross();
    int makeBinNumber();
    
    int lSpeed;
    int rSpeed;
    void turnLeft();
    void turnRight();
    void halt();
    void forward();
  private:
  
    // VARIABLES
    
    int _leftPort; 
    int _middlePort; 
    int _rightPort; 
    int _backPort;
    
    int _white = 200; // white is less than
    
    int state = 2;
    
    LineSensors currentSensors;
    
};

#endif
