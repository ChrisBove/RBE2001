/*

 Description:
    This code is written for the RBE 2002 B14 Final Project Robot. 
    main_mega is the .ino file associated with the specific code for running this robot on a Mega.
    
    See the navigator class for robot functions.
    
    Pins are as defined below this comment block.
    
  Authors: Christopher Bove, Christopher Ellen, Sami Neeno, and Daniel Pongratz
  
  Date last modified: 12-19-2014
  
  NOTE:  This code requires use of Arduino 1.5.8 or higher in order to compile.
  
  
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
  Serial.begin(115200); // start serial comm
  lola.setupNavigator(); // setup procedures.
  Serial.println("Setup complete.");
}

void loop() {
  lola.service(); // service the navigator
}
