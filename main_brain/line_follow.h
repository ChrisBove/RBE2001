#ifndef line_follow_h
#define line_follow_h

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
    int doLineFollowTillCross();
    int allOnCross();
    int makeBinNumber();
  private:
  
    // VARIABLES
    
    int _leftPort; 
    int _middlePort; 
    int _rightPort; 
    int _backPort;
    
    int _white = 200; // white is less than
    
    LineSensors currentSensors;
    
};

#endif
