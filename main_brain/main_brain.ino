// ***** INCLUDED LIBRARIEIS *******

// put them here...
#include <Servo.h>

#include <PPM.h>

#include "drive_train.h"
#include "vex_controller.h"
#include "line_follow.h"
#include "button.h"

Controller controller(2);
DriveTrain driveTrain(7, 6, 1, 0);

LineFollow follow(0, 1, 2, 3);
Button frontBumper(22);

void setup() {
  driveTrain.attachMotors();
  driveTrain.halt();
  
  frontBumper.setupButton();
}

void loop() {
  static int isInitialized = false;
  static int weMadeIt = false;
  static int result = 0;
    if(!((controller.getControllerChannel(5)) > 130 || (controller.getControllerChannel(5) < 50)) ) {
      driveTrain.moveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
      follow.resetCrossCount();
      isInitialized = false;
      weMadeIt = false;
      result = 0;
    }
    else
    {
//      if (!frontBumper.isBumped())
//      follow.doLineFollowTillCross(driveTrain);
//      else driveTrain.halt();
      if (!weMadeIt)
        result = follow.stopOnCrossing(driveTrain, 3);
      
      if (result == 0)
        weMadeIt = false; // wait until we make it
      else {
        // we made it!
        weMadeIt = true;
        if(!isInitialized) {
          driveTrain.setTime();
          isInitialized = true;
        }
        driveTrain.turn45(true); // turn right
    }
    
//    if(!((controller.getControllerChannel(6)) > 130 || (controller.getControllerChannel(6) < 50)) ) {
//      follow.resetCrossCount();
//    }
//    else
//    {
//      if (follow.stopOnCrossing(driveTrain, 3) == 0)
//        ; // wait until we make it
//      else {
//        // we made it! YAAAAAAY!
//        
//      }
    }
}
