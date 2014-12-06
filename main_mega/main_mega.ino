/*
  This program runs on the Arduino Mega to control robot
  functions.
  
*/

// ***** INCLUDED LIBRARIEIS *******
// 3rd party libraries
#include <Servo.h>

// user-defined libraries, contained in root
#include "drive_train.h"
#include "sensor_mast.h"

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

void setup() {
  driveTrain.attachMotors(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  driveTrain.forward();
////  driveTrain.turn(0, 180);
//  delay(1000);
//  driveTrain.reverse();
//  delay(1000);
//  driveTrain.halt();

  Serial.print("PW: ");
  Serial.print(sensorMast.getDigitalDistance());
  Serial.print("\t Analog: ");
  Serial.println(sensorMast.getAnalogDistance());
  
  delay(10);
}
