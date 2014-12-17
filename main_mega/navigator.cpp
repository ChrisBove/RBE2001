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
//VFH vfh; //&myGrid, &myHist);
LCD my_lcd;
VirtualBumper virtualBumper(ULTRA_PIN, DIG_ULTRA_PIN);
//IMUDriver imu;


Navigator::Navigator() {
  // maybe stuff in some pointers to other objects that are passed.
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
//  if(serviceCannon){
//    cannonControl.service();
//  }

  //cannonControl.service();
//  imu.service();
  
  // function that now calls the state machine for Navigator
  chooseAction();

}

void Navigator::chooseAction() {
  // check conditions necessary for switching controls on the state machine
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
    
    case TEST:
      
      if (driveTrain.rotateX(PI))
        state = CALC_POSITION;
      break;
      
    case LOCATE_CANDLE:
      virtualBumper.steerMe(driveTrain);
      
      if(sensorMast.isFire()) {
        state = SPIN_TO_CANDLE;
        driveTrain.halt();
        sensorMast.indicateNear();
        isFirstTime = true;
        Serial.println("Candle found");
      }
      break;
      
    case SPIN_TO_CANDLE:
      if (centerFlame()){
        driveTrain.halt();
       
        state = SPIN_MORE;
        isFirstReCenter = true;
        Serial.println("Done spinning");
      }
      
      break;
    
    case SPIN_MORE:
      if (centerMore()){
        driveTrain.halt();
       
        state = CALC_POSITION;
       
        Serial.println("Done spinning more");
      }
      
      break;
    
    case GET_CLOSE_TO_CANDLE:
      if (goToFlame()){
        driveTrain.halt();
        state = CALC_POSITION;
       
        Serial.println("Done getting close");
      }
      
      break;
    
    case CALC_POSITION:
      candle_Position();
      cannonControl.resetCannon();
      Serial.println("Done calculating our position");
      state = EXTINGUISH;
      break;
    
    case EXTINGUISH:
      cannonControl.cannonOP();
//      Serial.println("Putting out candle");
//serviceCannon = true;
      if(cannonControl.returnResult()){
        Serial.println("Candle is out, mission success");
        state = RETURN;
        
      }
      
      break;
      
    case RETURN:
      sensorMast.indicateNear();
      sensorMast.indicateWin();
      
      break;
    
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
  float d = virtualBumper.getDistance()+6.5; // distance + distance from sensor to center of robot
  d = 24;
//  float aim; 
//  float x_ref; 
//  float y_ref; 
//now the x-y coordinates after the switch from "get close" to "extinguish" procedure

  // to get X coord: cos(theta) * d
  // to get Y coordinate: sin(theta) *d
  float xPos = sin(head) * d;
  float yPos = cos(head) *d;
  // add X and Y to the current position
  my_lcd.printLocationNow(driveTrain.getX() + xPos, driveTrain.getY() + yPos, 7.89);
  
  
//  if ( 2*PI>= head > 1.5*PI){
//    aim = head - 2*PI;
//    x_ref = d*cos(aim);
//    y_ref = d*sin(aim);
//  }
//  else if (1.5*PI >= head > PI){
//    aim = -1*(head - PI);
//    x_ref = -d*cos(aim);
//    y_ref = d*sin(aim);
//  }
//  else if (PI >= head > 0.5*PI){
//    aim = head - 0.5*PI; 
//    x_ref = -d*cos(aim);
//    y_ref = d*sin(aim);
//  }
//  else{
//    x_ref = d*cos(aim);
//    y_ref = d*sin(aim);
//  }
////Find the z-coordinate: talk to chris E for the extraction function.
////  float c_z = 11.5 + d*tan(cannon.giveAngle());
//
////-------------------------------------------------------------------
//  float c_x = x_coord + x_ref;
//  float c_y = y_coord + y_ref; 
//  float c_z = 0.00;
//  my_lcd.printLocationNow(c_x, c_y, c_z);
}

void Navigator::doBumper() {
  // bumper controls robot's motion
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
