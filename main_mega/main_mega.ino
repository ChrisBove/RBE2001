/*
  This program runs on the Arduino Mega to control robot
  functions.
  
*/

// ***** INCLUDED LIBRARIEIS *******
// 3rd party libraries
#include <Servo.h>
#include <Encoder.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <LSM303.h>

// user-defined libraries, contained in root
#include "navigator.h"

// create navigator class, must have a British accent to be believable
Navigator lola;


void setup() {
  Serial.begin(115200);
  lola.setupNavigator();
  Serial.println("Setup complete.");
}

void loop() {
  lola.service();
  // put your main code here, to run repeatedly:
//  driveTrain.forward();
////  driveTrain.turn(0, 180);
//  delay(1000);
//  driveTrain.reverse();
//  delay(1000);
//  driveTrain.halt();

//  Serial.print("PW: ");
//  Serial.print(sensorMast.getDigitalDistance());
//  Serial.print("\t Analog: ");
//  Serial.println(sensorMast.getAnalogDistance());
  
//  Serial.println(sensorMast.getFlameReading());
  
//  sensorMast.setServoSpin();
//  sensorMast.center();
}
