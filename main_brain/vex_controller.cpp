#include "Arduino.h"
#include "vex_controller.h"

#include "PPM.h"

PPM ppm(2);

Controller::Controller(int pin) {
  Serial.begin(115200);
}

int Controller::getControllerChannel(int channel) {
  return ppm.getChannel(channel);
}

bool Controller::isDownPressed(int channel) {
  return ppm.getChannel(channel) > 130;
}

bool Controller::isUpPressed(int channel) {
  return ppm.getChannel(channel) < 50;
}
