#ifndef line_follow_h
#define line_follow_h

class LineFollow {
  
  public:
    LineFollow(int leftPort, int middlePort, int rightPort, int backPort);
    
    struct LineSensors {
      int left;
      int middle;
      int right;
      int back;
    };
    
  private:
  
    // VARIABLES
    
    int _leftPort; 
    int _middlePort; 
    int _rightPort; 
    int _backPort;
    
    LineSensors currentSensors;
};

#endif
