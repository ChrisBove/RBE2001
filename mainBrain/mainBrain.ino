// ***** INCLUDED LIBRARIEIS *******

// put them here...
#include <Servo.h>

#include <PPM.h>

#include "drive_train.h"
#include "VexController.h"

Controller controller(2);
DriveTrain driveTrain(7, 6, true, false);

void setup() {
  // put setup calls here
  

}

void loop() {
  // put main loop calls here
  
  driveTrain.MoveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
}
