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


Navigator::Navigator() {
  // maybe stuff in some pointers to other objects that are passed.
}

void Navigator::setupNavigator() {
  driveTrain.setupDriveTrain(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  sensorMast.setupMast();
  cannonControl.setupCannon();
  my_lcd.setupLCD();
  
//  myGrid = vfh.grid_init(50, 1);
//  myHist = vfh.hist_init(2, 20, 10, 5);
  
  lastServoPos = sensorMast.getServoAngle();
  measureCount = 0;
}

void Navigator::service() {
  driveTrain.service();
  sensorMast.service();
  //cannonControl.service();
  
  // function that now calls the state machine for Navigator
  chooseAction();

}

void Navigator::chooseAction() {
  // check conditions necessary for switching controls on the state machine
  
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
        Serial.println("Candle found");
      }
      break;
    case SPIN_TO_CANDLE:
      if (centerFlame()){
        driveTrain.halt();
       
        state = GET_CLOSE_TO_CANDLE;
       
        Serial.println("Done spinning");
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
      Serial.println("Done calculating our position");
      state = EXTINGUISH;
      break;
    
    case EXTINGUISH:
      cannonControl.service();
      Serial.println("Putting out candle");
      if(cannonControl.returnResult()){
        Serial.println("Candle is out, mission success");
        state = RETURN;
        
      }
      
      break;
      
    case RETURN:
      
      
      break;
  }
}

void Navigator::candle_Position(){
  float x_coord = driveTrain.getX();
  float y_coord = driveTrain.getY();
  float head = driveTrain.getHeading(); 
  float d = virtualBumper.getDistance()+6.5; 
  
  float aim; 
  float x_ref; 
  float y_ref; 
//now the x-y coordinates after the switch from "get close" to "extinguish" procedure
  
  if ( 2*PI>= head > 1.5*PI){
    aim = head - 2*PI;
    x_ref = d*cos(aim);
    y_ref = d*sin(aim);
  }
  else if (1.5*PI >= head > PI){
    aim = -1*(head - PI);
    x_ref = -d*cos(aim);
    y_ref = d*sin(aim);
  }
  else if (PI >= head > 0.5*PI){
    aim = head - 0.5*PI; 
    x_ref = -d*cos(aim);
    y_ref = d*sin(aim);
  }
  else{
    x_ref = d*cos(aim);
    y_ref = d*sin(aim);
  }
//Find the z-coordinate: talk to chris E for the extraction function.
//  float c_z = 11.5 + d*tan(cannon.giveAngle());

//-------------------------------------------------------------------
  float c_x = x_coord + x_ref;
  float c_y = y_coord + y_ref; 
  float c_z = 0.00;
  my_lcd.printLocationNow(c_x, c_y, c_z);
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

bool Navigator::centerFlame()
{ 
  if (sensorMast.isFire())
  { 
     if(driveTrain.getHeadingDeg()+103> sensorMast.getServoAngle())
       { 
         sensorMast.center();
         sensorMast.freeze();
         driveTrain.moveMotors(25, -25);
         if (sensorMast.isFire() && fireCount==1)
         {
//           Serial.println( fireCount);
           return true;  
         }
         fireCount++;
       }
     if(driveTrain.getHeadingDeg()+103< sensorMast.getServoAngle())
       {  
         sensorMast.center();
         sensorMast.freeze();
         driveTrain.moveMotors(-25, 25);
         if (sensorMast.isFire() && fireCount==1)
         {
           return true;
         }
         fireCount++;
       }
      if(102< sensorMast.getServoAngle()&& 104> sensorMast.getServoAngle()&& sensorMast.isFire() &&fireCount==0)
       {
         sensorMast.freeze();
         return true;
       } 
  } 
  else
  {
    return false;
  }
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
