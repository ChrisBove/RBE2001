/**
 ********************************************************************************************************
 * @file    vex_controller.cpp
 * @brief   vex_controller class methods
 * @details handles controller related functions
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "vex_controller.h"

#include "PPM.h"

PPM ppm(2); // create PPM object

Controller::Controller(int pin) {
  Serial.begin(115200); // begin serial comms
}

int Controller::getControllerChannel(int channel) {
  return ppm.getChannel(channel); // return the channelreading
}

bool Controller::isDownPressed(int channel) {
  return ppm.getChannel(channel) > 130; // if channel is greater than some threshold, button is pressed down
}

bool Controller::isUpPressed(int channel) {
  return ppm.getChannel(channel) < 50; // if channel is less than some threshold, button is pressed up
}
