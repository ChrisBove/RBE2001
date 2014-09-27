#include "Arduino.h"
#include "VexController.h"

#include "PPM.h"

PPM ppm(2);

Controller::Controller(int pin) {
  Serial.begin(115200);
//  PPM ppm(2); // initializes a PPM object named ppm onto pin #2
}

int Controller::getControllerChannel(int channel) {
  return ppm.getChannel(channel);
}
