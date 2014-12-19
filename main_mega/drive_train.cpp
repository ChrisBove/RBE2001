/**
 ********************************************************************************************************
 * @file    drive_train.cpp
 * @brief   drive train control methods
 * @details Used to control the drivetrain, turn, etc.
 
   Encoder localization functions modified from:
     odometer.c - Copyright (C) 2000, Dafydd Walters
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "drive_train.h"

#include <Servo.h>  // import libraries
#include <Encoder.h>

// create drivetrain encoders
Encoder right_encoder (20, 3);
Encoder left_encoder (19, 2);

#define WHEEL_DIAMETER 2.75
#define PULSES_PER_REVOLUTION 3200.0
#define AXLE_LENGTH 9.63

DriveTrain::DriveTrain(int leftPin, int rightPin, bool leftInverted, bool rightInverted) {
  _leftPin = leftPin;
  _rightPin = rightPin;
  
  // sets offsets for inverted motors
  if (leftInverted)
    leftInversion = -1;
  if (rightInverted)
    rightInversion = -1;
    
}

void DriveTrain::setupDriveTrain() {
  left.attach(_leftPin, 1000, 2000);  // limit PWM vals
  right.attach(_rightPin, 1000, 2000);
  initialize_odometry(); // init odom
}

void DriveTrain::service() {
  odometer_thread(); // run odometry thread
//  if (update_pos) // if time to update pose, do a transform
//    transform();
}

// pass in values between -90 and 90
void DriveTrain::moveMotors(int leftVal, int rightVal) {
  // if we're good to move, move the motors - yo, don't use this, as right now 
  // we have a logic loop. SOOOOO DUMB.
//  if(shouldMove) {
    left.write((leftVal*leftInversion) + 90);    // go from -90-90 to 0-180
    right.write((rightVal*rightInversion) + 90);
//  }
//  else
//    halt(); // call this, and it just calls this function again
}

void DriveTrain::moveInDir(int dir) {
  // we get our direction -90 to 90
  // map direction to motor speeds
  // 0 - both go forward at roaming speed
  // -45 - 
  moveMotors(roamSpeed + dir, roamSpeed - dir);
}

void DriveTrain::initialize_odometry()
{
  current_position.x = 0.0;
  current_position.y = 0.0;
  current_position.theta = 0.0;
}

void DriveTrain::odometer_thread()
{
  
  // Encoder localization functions modified from:
  //   odometer.c - Copyright (C) 2000, Dafydd Walters
  
  float dist_left;
  float dist_right;
  int left_ticks;
  int right_ticks;
  float expr1;
  float cos_current;
  float sin_current;
  float right_minus_left;
  float MUL_COUNT;

  MUL_COUNT  = PI * WHEEL_DIAMETER / PULSES_PER_REVOLUTION;
  
  
  left_ticks = left_encoder.read();
  // only stop interrupts if there is a need to wipe encoders
  if (left_ticks != 0) {
    noInterrupts();
    left_encoder.write(0);
    interrupts();
  }
  right_ticks = -right_encoder.read();
  // only stop interrupts if there is a need to wipe encoders
  if (right_ticks != 0) {
    noInterrupts();
    right_encoder.write(0);
    interrupts();
  }

  if (left_ticks != 0 || right_ticks != 0) {
    dist_left = (float)left_ticks * MUL_COUNT;
    dist_right = (float)right_ticks * MUL_COUNT;

    cos_current = cos(current_position.theta);
    sin_current = sin(current_position.theta);

    if (left_ticks == right_ticks)
    {
      /* Moving in a straight line */
      current_position.x += dist_left * cos_current;
      current_position.y += dist_left * sin_current;
    }
    else
    {
      /* Moving in an arc */
      expr1 = AXLE_LENGTH * (dist_right + dist_left)
              / 2.0 / (dist_right - dist_left);

      right_minus_left = dist_right - dist_left;

      current_position.x += expr1 * (sin(right_minus_left /
                            AXLE_LENGTH + current_position.theta) - sin_current);

      current_position.y -= expr1 * (cos(right_minus_left /
                            AXLE_LENGTH + current_position.theta) - cos_current);

      /* Calculate new orientation */
      current_position.theta += right_minus_left / AXLE_LENGTH;
      unboundedTheta += right_minus_left / AXLE_LENGTH; // don't bound this one
      /* Keep in the range -PI to +PI */
      while(current_position.theta > PI)
        current_position.theta -= (2.0*PI);
      while(current_position.theta < -PI)
        current_position.theta += (2.0*PI);
        
    }
    
    update_pos = true;
  }
  else update_pos = false;
    
}

float DriveTrain::getX() {
  return current_position.x;
}

float DriveTrain::getY() {
  return current_position.y;
}

float DriveTrain::getHeading() {
  return current_position.theta;
}

float DriveTrain::getUnboundedHeading() {
  return unboundedTheta;
}

void DriveTrain::transform(){
  transformed.x = -current_position.y;
  transformed.y = current_position.x;
  transformed.theta = current_position.theta - (PI/2.0);
}

void DriveTrain::halt(){
  moveMotors(leftStop, rightStop);
}

void DriveTrain::forward() {
  moveMotors(90, 90);
}

void DriveTrain::reverse() {
  moveMotors(-90, -90); // true reverse!
}

int DriveTrain::getHeadingDeg() {
  return (transformed.theta *(180.0/PI));
}


//****-------sensor-controlled rotation and translation-------*******//
bool DriveTrain::backupX(float inches){
  // if this is a new command, set our target
  if(new_pos_command) {
//    Serial.println("New command");
    pos_target_x = getX() + ( inches * cos(getHeading() )); // get x coordinate, add to delta x
    pos_target_y = getY() + ( inches * sin(getHeading() )); // get y, add to delta y
    new_pos_command = false;
  }
  else {
    // else, our target is current
    
    moveMotors(-30,-30);
    
    float xDif = abs(abs(getX()) - abs(pos_target_x));
    bool xInBounds =  xDif <= pos_error; // in bounds if error is less than posError
    float yDif = abs(abs(getY()) - abs(pos_target_y));
    bool yInBounds = yDif <= pos_error; // in bounds if error is less than posError
    // if both coordinates in bounds, stop and say we're in bounds
    if ((xInBounds && yInBounds) || ( sqrt((xDif*xDif) + (yDif*yDif)) >= inches )){
  //    Serial.println("backup complete");
      halt();
      new_pos_command = true;
      return true;
    }
  }
  return false;
}

bool DriveTrain::rotateX(float spin){
  // if new command, calc target theta
  if (new_command){
    target = spin + unboundedTheta;
    new_command = false;
  }
  // choose motor direction
  if (spin < 0){
    moveMotors(25,-25); 
  }
  else if (spin > 0){
    moveMotors(-25,25);
  }
  // if we are within the bounds, halt and say we're done
  if( abs(abs(target) - abs(unboundedTheta)) <= 0.1) {
    halt();
    new_command = true; 
    return true; 
  }
  return false; 
}


