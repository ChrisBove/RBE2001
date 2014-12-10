/**
 ********************************************************************************************************
 * @file    navigator.h
 * @brief   navigator class
 * @details controls the robot using sensors and the drivetrain
 ********************************************************************************************************
 */
 
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "navigator.h"
#include "drive_train.h"
#include "sensor_mast.h"
#include "sonic_assembler.h"
#include "vfh.h"
#include "cliff.h"


// ************* CONSTANTS AND PINS ***************
#define leftMotorPin    10
#define rightMotorPin   11
#define mastServoPin    9
#define redFlamePin     0
#define ultraPin        1
#define digUltraPin     22

// *************** instantiate class objects **************
DriveTrain driveTrain(leftMotorPin, rightMotorPin, true, false); // left motor inverted, right not
SensorMast sensorMast(mastServoPin, ultraPin, redFlamePin, digUltraPin);


Navigator::Navigator() {
  // maybe stuff in some pointers to other objects that are passed.
}

void Navigator::setupNavigator() {
  driveTrain.attachMotors(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  sensorMast.setupMast();
}

void Navigator::service() {
  
}



// TODO
