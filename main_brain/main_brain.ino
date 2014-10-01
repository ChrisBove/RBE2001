// ***** INCLUDED LIBRARIEIS *******

// put them here...
#include <Servo.h>

#include <PPM.h>

#include "drive_train.h"
#include "vex_controller.h"
#include "line_follow.h"

Controller controller(2);
DriveTrain driveTrain(7, 6, 1, 0);

LineFollow follow(0, 1, 2, 3);

void setup() {
  
  driveTrain.attachMotors();
  driveTrain.halt();
}

void loop() {
  if(!((controller.getControllerChannel(5)) > 130 || (controller.getControllerChannel(5) < 50)) )
    driveTrain.moveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
  else
  {
    follow.doLineFollowTillCross(driveTrain);
  }
}
