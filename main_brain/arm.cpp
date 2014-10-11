#include "arm.h"
#include "button.h"

Servo arm;


Arm::Arm(int motorPin, int potPin, Button& frontLimit, Button& backLimit) {
  _motorPin = motorPin;
  _potPin = potPin;
}
    
void Arm::setupArm() {
  arm.attach(_motorPin);
  arm.write(90);
}
