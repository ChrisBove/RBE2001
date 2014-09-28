// ***** INCLUDED LIBRARIEIS *******

// put them here...
#include <Servo.h>

#include <PPM.h>

#include "drive_train.h"
#include "vex_controller.h"
#include "line_follow.h"

Controller controller(2);
DriveTrain driveTrain(7, 6, 1, 0);

void setup() {
  
  driveTrain.AttachMotors();

}

void loop() {
  
  driveTrain.MoveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );

}
