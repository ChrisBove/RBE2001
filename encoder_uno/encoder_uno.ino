/*
  This code is to be run on the arduino Uno connected to the
  motor encoders on a differentially driven robot base.
  
*/

#include <Encoder.h>

Encoder left (2, 3);
// TODO implement other encoder

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

long oldPosition = -999;

void loop() {
  // put your main code here, to run repeatedly:
  long newPosition = left.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
