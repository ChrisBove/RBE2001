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
#include "lcd.h"

// ************* CONSTANTS AND PINS ***************
#define LEFT_MOTOR_PIN    10
#define RIGHT_MOTOR_PIN   11
#define MAST_SERVO_PIN    9
#define RED_FLAME_PIN     0
#define ULTRA_PIN        1
#define DIG_ULTRA_PIN     22

// globals:
VFH::grid_t * myGrid;
VFH::hist_t * myHist;

// *************** instantiate class objects **************
DriveTrain driveTrain(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN, true, false); // left motor inverted, right not
SensorMast sensorMast(MAST_SERVO_PIN, ULTRA_PIN, RED_FLAME_PIN, DIG_ULTRA_PIN);
SonicAssembler assembler;
VFH vfh(); //&myGrid, &myHist);

//myGrid = vfh.grid_init(50,10);
//myHist = vfh.hist_init(2, 20, 10, 5);


Navigator::Navigator() {
  // maybe stuff in some pointers to other objects that are passed.
}

void Navigator::setupNavigator() {
  driveTrain.setupDriveTrain(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  sensorMast.setupMast();
}

void Navigator::service() {
  driveTrain.service();
  sensorMast.service();
//  Serial.print("Grid update: ");
//  Serial.print(vfh.grid_update(&myGrid, driveTrain.getX(), driveTrain.getY(), assembler.assemble(sensorMast.getServoAngle(), sensorMast.getDistance())) );
////  Serial.print("\t Hist update: ");
//  vfh.hist_update(&myHist, &myGrid);
//  Serial.print("\t Dir: ");
//  
//  Serial.println(vfh.calculate_direction(&myHist, 90));
  
//  Serial.println(sensorMast.getServoAngle());
//  delay(10);
  driveTrain.halt();
//  driveTrain.moveInDir(0);
}



// TODO
