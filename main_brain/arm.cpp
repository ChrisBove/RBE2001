#include "arm.h"
Servo arm;

Arm::Arm(int motorPin, int potPin) {
  _motorPin = motorPin;
  _potPin = potPin;
}
    
void Arm::setupArm() {
  arm.attach(_motorPin);
  arm.write(90);
}
