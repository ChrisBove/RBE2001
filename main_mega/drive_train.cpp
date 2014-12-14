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
  
  // following sets offsets, but it is not fully implemented
  if (leftInverted)
    leftInversion = -1;
  if (rightInverted)
    rightInversion = -1;
    
}

void DriveTrain::setupDriveTrain() {
  left.attach(_leftPin, 1000, 2000);
  right.attach(_rightPin, 1000, 2000);
  initialize_odometry();
}

void DriveTrain::service() {
  odometer_thread();
  if (update_pos)
    transform();
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
  
  
  //  enable_interrupts(0);         /* Ensure we don't lose any odometer counts */
//  noInterrupts();
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
//    left_count = 0;
//    right_count = 0;
//  interrupts();
//    enable_interrupts(1);

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
      unboundedTheta += right_minus_left / AXLE_LENGTH;
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
  return transformed.x;
}

float DriveTrain::getY() {
  return transformed.y;
}

float DriveTrain::getHeading() {
  return transformed.x;
}

void DriveTrain::transform(){
  transformed.x = -current_position.y;
  transformed.y = current_position.x;
  transformed.theta = current_position.theta - (PI/2.0);
}

void DriveTrain::turnLeft(dirTravel dir){
  if (dir == FORWARD) {
    moveMotors(10, 50);
    // left should be slower or backwards
  }
  else { // reverse turn right
    moveMotors(-10, 120);
  }
}

void DriveTrain::turnRight(dirTravel dir) {
  if (dir == FORWARD) {
    moveMotors(50, 10);
  }
  else { // reverse turn left
    moveMotors(75, 80);
  }
}

void DriveTrain::sharpTurnLeft (dirTravel dir) {
  if (dir == FORWARD)
    moveMotors(0, 50);
  // reverse direction is broken
//  else 
//    turn(75, 85); // reverse sharp turn RIGHT
}

void DriveTrain::sharpTurnRight(dirTravel dir) {
  if (dir == FORWARD)
    moveMotors(110, 100);
  // reverse direction is broken
//  else
//    turn(105, 120); // reverse sharp turn left

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

void DriveTrain::turn(int lval, int rval) {
  left.write(lval);
  right.write(rval);
}

void DriveTrain::setTime() {
  startTime = millis();
}

// returns 1 when the turn is done
bool DriveTrain::turn45(bool isRight) {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 550) { // if not gone the time, keep turning
    if (isRight)
      turn(110, 110);
    else
      turn(70, 70);
    return false;
  }
  else {
    halt(); // stop when time has expired
    return true;
  }
}

bool DriveTrain::turn180(bool isRight) {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 1200) { // if not gone the time, keep turning
    if (isRight) {
      turn(110, 110);
    }
    else {
      turn(70, 70);
    }
    return false;
  }
  else { // stop when time expires
    halt();
    return true;
  }
}

bool DriveTrain::turnAround(bool isRight) {
  switch(turnState) {
      // turn until the line sensors only see white
      case TURN_OFF_LINE:
        if (isRight) {
          moveMotors(110, 110);
        }
        else {
          moveMotors(70, 70);
        }
        if ((analogRead(0) < 200) && (analogRead(1) < 200) && (analogRead(2) < 200)) // ALL on white
          turnState = TURN_TILL_LINE;
         break;
       // turn other way until sensors detect black
       case TURN_TILL_LINE:
         if (isRight) {
           moveMotors(110, 110);
         }
         else {
           moveMotors(70, 70);
         }
         if ((analogRead(0) > 200) || (analogRead(1) > 200) || (analogRead(2) > 200)) { // any on black
           turnState = TURN_OFF_LINE;
           return true; // return that the turn is complete
         }
         break;
  }
  return false;
}

bool DriveTrain::backupForTime() {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 650) {
    reverse();
    return false;
  }
  else {
    halt();
    return true;
  }
}

// not debugged
int DriveTrain::backupABit() {
  switch (revState) {
    case INIT_BACKUP:
      setTime();
      revState = BACKUP;
      break;

    case BACKUP:
      bool result = backupForTime();
      if (result) {
        revState = INIT_BACKUP;
        return 1;
      }
      break;
  }
  return 0;
}

bool DriveTrain::forwardForTime() {
  int timeLapse = millis() - startTime;
  if (timeLapse <= 650) {
    moveMotors(110, 80);
    return false;
  }
  else {
    halt();
    return true;
  }
}

// not debugged
int DriveTrain::forwardABit() {
  switch (forwardState) {
    case INIT_FORWARD:
      setTime();
      forwardState = RUN_FORWARD;
      return false;
      break;
    case RUN_FORWARD:
      bool result = forwardForTime();
      if (result) {
        forwardState = INIT_FORWARD;
        return true;
      }
      return false;
      break;
  }
}


int DriveTrain::getHeadingDeg() {
  return (transformed.x *(180.0/PI));
}


//****-------sensor-controlled rotation and translation-------*******//
bool DriveTrain::backupX(float inches){
  // if this is a new command, set our target
  if(new_pos_command) {
    pos_target_x = getX() + ( inches * sin(getHeading() )); // get x coordinate, add to delta x
    pos_target_y = getY() + ( inches * cos(getHeading() )); // get y, add to delta y
    new_pos_command = false;
  }
  // else, our target is current
  
  moveMotors(-30,-30);
  
//  new_pos_command = false; 
  
  bool xInBounds = abs(abs(getX()) - abs(pos_target_x)) <= pos_error; // in bounds if error is less than posError
  bool yInBounds = abs(abs(getY()) - abs(pos_target_y)) <= pos_error; // in bounds if error is less than posError
  // if both coordinates in bounds,
  if (xInBounds && yInBounds){
    halt();
    new_pos_command = true;
    return true;
  }
  return false;
}

bool DriveTrain::rotateX(float spin){
  if (new_command){
    target = spin + unboundedTheta;
    new_command = false;
  }
  
  if (spin < 0){
  moveMotors(25,-25); 
  }
  else if (spin > 0){
  moveMotors(-25,25);
  }
  if( abs(abs(target) - abs(unboundedTheta)) <= 0.1) {
    halt();
    new_command = true; 
    return true; 
  }
  return false; 
}


