#ifndef vex_controller_h
#define vex_controller_h

#include "PPM.h"

class Controller {
  public:
    Controller(int pin);
    int getControllerChannel(int channel);
    bool isUpPressed(int channel);
    bool isDownPressed(int channel);
  private:
    
};

#endif
