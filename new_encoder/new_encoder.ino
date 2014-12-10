/*
  This code is to be run on the arduino Uno connected to the
  motor encoders on a differentially driven robot base.
  
*/

#include <Encoder.h>

Encoder right (2, 4);
Encoder left (3, 5);

bool update = true;

/***********************************************************/
/* odometer.c - Copyright (C) 2000, Dafydd Walters         */
/***********************************************************/


/********************/
/* define constants */
/********************/

/* User defined constants */
#define WHEEL_DIAMETER 0.06985
#define PULSES_PER_REVOLUTION 3200.0
#define AXLE_LENGTH 0.244602

/* Fixed constants */
//#define PI 3.14159


/*********************/
/* define structures */
/*********************/

struct position
{
  float x;        /* meter */
  float y;        /* meter */
  float theta;    /* radian (counterclockwise from x-axis) */
};


/********************/
/* global variables */
/********************/

struct position current_position;


/********************/
/* define functions */
/********************/

void initialize_odometry()
{
  current_position.x = 0.0;
  current_position.y = 0.0;
  current_position.theta = 0.0;
}


void odometer_thread()
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
  noInterrupts();
  left_ticks = left.read();
  right_ticks = -right.read();
  right.write(0);
  left.write(0);
//    left_count = 0;
//    right_count = 0;
  interrupts();
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
      /* Keep in the range -PI to +PI */
      while(current_position.theta > PI)
        current_position.theta -= (2.0*PI);
      while(current_position.theta < -PI)
        current_position.theta += (2.0*PI);
        
    }
    
    update = true;
  }
  else update = false;
    
}

struct position transformed;

void transform(){
  transformed.x = -current_position.y;
  transformed.y = current_position.x;
  transformed.theta = -current_position.theta;
//  float newX = -current_position.y;
//  current_position.y = newX;
}
  
 

void setup() {
  // put your setup code here, to run once:
  initialize_odometry();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  odometer_thread();
  
  if (update) {
    transform();
    Serial.print("Heading: ");
    Serial.print(transformed.theta);
    Serial.print("\t X: ");
    Serial.print(transformed.x);
    Serial.print("\t Y: ");
    Serial.println(transformed.y);
  }
    
}

