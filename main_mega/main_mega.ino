/*
  This program runs on the Arduino Mega to control robot
  functions.
  
*/

// ***** INCLUDED LIBRARIEIS *******
// 3rd party libraries
#include <Servo.h>

// user-defined libraries, contained in root
#include "drive_train.h"

// ************* CONSTANTS AND PINS ***************
#define leftMotorPin    4
#define rightMotorPin   5

// *************** instantiate class objects **************
DriveTrain driveTrain(leftMotorPin, rightMotorPin, true, 0); // left motor inverted, right not

void setup() {
  driveTrain.attachMotors(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
