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
#include "cannon_control.h"
//#include "vfh.h"
#include "lcd.h"
#include "virtual_bumper.h"
#include "imu_driver.h"

// ************* CONSTANTS AND PINS ***************
#define LEFT_MOTOR_PIN    10
#define RIGHT_MOTOR_PIN   11
#define MAST_SERVO_PIN    9
#define RED_FLAME_PIN     0
#define ULTRA_PIN         1
#define DIG_ULTRA_PIN     22
#define GRIPPER_PIN       8
#define SERVO_PIN         7
#define MOTOR_PIN         6
#define FLAME_PIN         A2
#define LED_indicator     25 
#define LED_WIN           27 

// *************** instantiate class objects **************
DriveTrain driveTrain(LEFT_MOTOR_PIN, RIGHT_MOTOR_PIN, true, false); // left motor inverted, right not
SensorMast sensorMast(MAST_SERVO_PIN, RED_FLAME_PIN, LED_indicator, LED_WIN);
SonicAssembler assembler;
CannonControl cannonControl(GRIPPER_PIN, SERVO_PIN, MOTOR_PIN, FLAME_PIN);
//VFH vfh;
LCD my_lcd;
VirtualBumper virtualBumper(ULTRA_PIN, DIG_ULTRA_PIN);
//IMUDriver imu;


Navigator::Navigator() {
  
}

void Navigator::setupNavigator() {
  Serial.println("start setup");
  driveTrain.setupDriveTrain(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  sensorMast.setupMast();
  cannonControl.setupCannon();
  my_lcd.setupLCD();
//  imu.setupIMU();
  Serial.println("end setup");
//  myGrid = vfh.grid_init(50, 1);
//  myHist = vfh.hist_init(2, 20, 10, 5);
  
  lastServoPos = sensorMast.getServoAngle();
  measureCount = 0;
}

void Navigator::service() {
  driveTrain.service();
  sensorMast.service();
//  imu.service();
  
  // function that now calls the state machine for Navigator
  chooseAction();

}

void Navigator::chooseAction() {
  // check conditions necessary for switching controls on the state machine
  
  // our IMU stopped working for setup. For now, we just commented this working code out.
  
//  Serial.println(imu.isTipped());
  // if we are not currently in tilt
//  if (state != TILT) {
//    // if IMU is tipped, change our state
//    if (imu.isTipped()) {
//      lastState = state;
//      state = TILT;
//    }
//  }
//  // if in tilt state, check to see if we're done tipping
//  else {
//    if (!imu.isTipped()) {
//      state = lastState;
//    }
//  }
  
  switch (state) {
    
    // this is just a case for unit testing different functions
    case TEST:
      Serial.println(virtualBumper.getAnalogDistance());
      if (sensorMast.isFire()) {
        state = SPIN_TO_CANDLE;
        driveTrain.halt();
        sensorMast.indicateNear();
        isFirstTime = true;
      }
      
//      Serial.println(driveTrain.getUnboundedHeading());
//        Serial.print(driveTrain.getX());
//        Serial.print("\t");
//        Serial.print(driveTrain.getY());
//        Serial.print("\t");
//        Serial.println(driveTrain.getUnboundedHeading());
//      if (driveTrain.rotateX(PI))
//        state = CALC_POSITION;
      break;
      
    // robot roams until finding the candle
    case LOCATE_CANDLE:
      virtualBumper.steerMe(driveTrain); // stear the drivetrain around obstacles
      
      // if we see some fire, stop and switch to spin to candle
      if(sensorMast.isFire()) {
        state = SPIN_TO_CANDLE;
        driveTrain.halt();
        sensorMast.indicateNear();
        isFirstTime = true;
        Serial.println("Candle found");
      }
      break;
    
    // robot spins to face the candle
    case SPIN_TO_CANDLE:
      // if the centering function is done, spin some more
      if (centerFlame()){
        driveTrain.halt();
        state = SPIN_MORE;
        isFirstReCenter = true;
        Serial.println("Done spinning");
      }
      
      break;
    
    // spins the robot even closer to flame
    case SPIN_MORE:
      if (centerMore()){
        driveTrain.halt();
       
        state = CALC_POSITION;
        Serial.println("Done spinning more");
      }
      
      break;
    
    // drives the robot closer to the candle, not implemented
    case GET_CLOSE_TO_CANDLE:
      if (goToFlame()){
        driveTrain.halt();
        state = CALC_POSITION;
       
        Serial.println("Done getting close");
      }
      
      break;
    
    // calculates the candle position relative to starting point and prints it
    case CALC_POSITION:
      candle_Position();
      cannonControl.resetCannon();
      Serial.println("Done calculating our position");
      state = EXTINGUISH;
      break;
    
    // extinguishes the candle
    case EXTINGUISH:
      cannonControl.cannonOP();
//      Serial.println("Putting out candle");

      if(cannonControl.returnResult()){
        Serial.println("Candle is out, mission success");
        state = RETURN;
        sensorMast.indicateNear();
        sensorMast.indicateWin();
      }
      
      break;
    
    // tries to return to the starting location through roaming
    case RETURN:
      // if we see fire while returning to home, go put it out.
      if(sensorMast.isFire() ) {
        state = SPIN_TO_CANDLE;
        driveTrain.halt();
        sensorMast.indicateNear();
        isFirstTime = true;
        break;
      }
      // if drivetrain is within +- 10in from home, call it quits
      if ( abs(driveTrain.getX() ) < 10 && abs(driveTrain.getY()) < 10)
        driveTrain.halt();
      // else keep roaming until you get back to base.
      else
        virtualBumper.steerMe(driveTrain);
      break;
    
    // takes over in case we start tipping - stop everything
    case TILT:
      driveTrain.halt();
      sensorMast.freeze();
      sensorMast.indicateNear();
      break;
  }
}

void Navigator::candle_Position(){
//  float x_coord = driveTrain.getX();
//  float y_coord = driveTrain.getY();
  float head = driveTrain.getUnboundedHeading(); 
  float d = virtualBumper.getAnalogDistance()+6.5; // distance + distance from sensor to center of robot

  // to get X coord: cos(theta) * d
  // to get Y coordinate: sin(theta) *d
  float xPos = cos(head) * d;
  float yPos = sin(head) *d;
  // add X and Y and Z to the current position
  my_lcd.printLocationNow(driveTrain.getX() + xPos, driveTrain.getY() + yPos, sin(cannonControl.giveAngle()) * d );
  
}

void Navigator::doVFH(){
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
}

bool Navigator::centerFlame() {
  if (isFirstTime) {
    // if greater than 90, turn left
    if(sensorMast.getServoAngle() > sensorMast.getServoCenter()) {
      driveTrain.moveMotors(-20, 20);
      isLeft = true;
    }
    else { // not greater than 90, so if perfect halt, otherwise go other direction
      if(sensorMast.getServoAngle() < sensorMast.getServoCenter()) {
        driveTrain.moveMotors(20, -20);
        isLeft = false;
      }
      else {
        driveTrain.halt();
      }
    }
    sensorMast.center();
    sensorMast.freeze();
    isFirstTime = false;
    haveSeenNoFire = false;
    return false;
  }
  else {
    if (!haveSeenNoFire) {
      if(!sensorMast.isFire())
        haveSeenNoFire = true;
    }
    else if (sensorMast.isFire()) {
      driveTrain.halt();
      return true;
    }
  }
  return false;
}

bool Navigator::centerMore() {
  int myReading = sensorMast.getFlameReading();
  // if its our first time, set this as our first reading
  if (isFirstReCenter) {
    lastFlameVal = myReading;
    if (isLeft)
      driveTrain.moveMotors(-19, 19);
    else 
      driveTrain.moveMotors(19, -19);
    isFirstReCenter = false;
  }
  // if the flame val starts increasing, we are moving away from the candle, so stop
  else if(myReading - lastFlameVal  > 0) {
    driveTrain.halt();
    return true;
  }
  lastFlameVal = myReading;
  return false;
}

bool Navigator::goToFlame()
{ driveTrain.moveMotors(20, 21);

if (sensorMast.isFire()==false)
{
  driveTrain.halt();
  centerFlame();
//  Serial.println(virtualBumper.getAnalogDistance());
}

if ( virtualBumper.getAnalogDistance()<8)
{
  
  driveTrain.halt();
  return true;
}
return false;
}
