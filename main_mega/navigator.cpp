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
//#include "vfh.h"
#include "lcd.h"
#include "cliff.h"

// ************* CONSTANTS AND PINS ***************
#define LEFT_MOTOR_PIN    10
#define RIGHT_MOTOR_PIN   11
#define MAST_SERVO_PIN    9
#define RED_FLAME_PIN     0
#define ULTRA_PIN        1
#define DIG_ULTRA_PIN     22
#define LeftLight       10
#define RightLight      11
#define LED_indicator    5 // PROBABLY WRONG
#define LED_WIN          6 // PROBABLY WRONG

// *************** instantiate class objects **************
DriveTrain driveTrain(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN, true, false); // left motor inverted, right not
SensorMast sensorMast(MAST_SERVO_PIN, ULTRA_PIN, RED_FLAME_PIN, DIG_ULTRA_PIN, LED_indicator, LED_WIN);
SonicAssembler assembler;
//VFH vfh; //&myGrid, &myHist);
LCD my_lcd;
CliffDetector cliffDetect(RightLight, LeftLight);


Navigator::Navigator() {
  // maybe stuff in some pointers to other objects that are passed.
}

void Navigator::setupNavigator() {
  driveTrain.setupDriveTrain(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  sensorMast.setupMast();
  my_lcd.setupLCD();
  
//  myGrid = vfh.grid_init(50, 1);
//  myHist = vfh.hist_init(2, 20, 10, 5);
  
  lastServoPos = sensorMast.getServoAngle();
  measureCount = 0;
}

void Navigator::service() {
  driveTrain.service();
  sensorMast.service();
  
  // TODO - add a function that now calls the state machine for Navigator
  
  // TODO - check for flame presence
  
  // do some navigation
  
  
  // stuff consequetive readings into an array
  /*
  // every 5 degrees of servo rotation, take a reading
  int pos = sensorMast.getServoAngle();
  if (abs(lastServoPos - pos) >=5) {
    lastServoPos = pos;
    assembler.assembleInArray(measureCount, sensorMast.getServoAngle(), sensorMast.getDistance());
    measureCount ++;
  }
  
  // if we have spun 90 degrees, stuff those measurements into the grid and process.
  if (measureCount >= 18) { //assembler.arraySize-1) {
    for (int i = 0; i < assembler.arraySize; i ++) {
      if (assembler.measure[i].distance != 0)
        vfh.grid_update(myGrid, driveTrain.getX(), driveTrain.getY(), assembler.measure[i]);
    }
    vfh.hist_update(myHist, myGrid);
    Serial.println(vfh.calculate_direction(myHist, 90));
    measureCount = 0;
    assembler.clearArray();
  }
  */ 
  
//  if (readings 
//  Serial.print("Grid update: ");
//  Serial.print(vfh.grid_update(myGrid, driveTrain.getX(), driveTrain.getY(), assembler.assemble(sensorMast.getServoAngle(), sensorMast.getDistance())) );
////  Serial.print("\t Hist update: ");
//  vfh.hist_update(myHist, myGrid);
//  Serial.print("\t Dir: ");
//  
//  Serial.println(vfh.calculate_direction(myHist, 90));
  
//  Serial.println(sensorMast.getServoAngle());
//  Serial.print(sensorMast.getDistance());
//  Serial.print("\t ");
//  Serial.println(sensorMast.getServoAngle());

//  delay(10);
//  driveTrain.halt();
//  driveTrain.moveInDir(0);

centerFlame();


}

void Navigator::centerFlame()
{
  if (sensorMast.isFire()==true)
  {
 
     if(driveTrain.getHeadingDeg()+13< sensorMast.getServoAngle())
     { 
       sensorMast.center();
       sensorMast.freeze();
       driveTrain.moveMotors(20, -20);
   
      }
    else if(driveTrain.getHeadingDeg()+13> sensorMast.getServoAngle())
     {  
       sensorMast.center();
       sensorMast.freeze();
      driveTrain.moveMotors(-20, 20);
      
     }
 
   else 
   {
   sensorMast.freeze();
   }
    
  }

  
}

// TODO
