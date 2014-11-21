/*
  This code is to be run on the arduino Uno connected to the
  motor encoders on a differentially driven robot base.
  
*/

#include <Encoder.h>

Encoder left (2, 3);
Encoder right (3, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}

long l_oldPosition = -999;
long r_oldPosition = -999;
float theta = 0;
float x = 0;
float y = 0;
#define radius 1.375;    // radius of drive wheels
#define distance 9.63;    // separation between wheels
#define ticksPerRot 3200;  // num encoder ticks for full rot.

void loop() {
  // put your main code here, to run repeatedly:
  long l_newPosition = left.read();
  long r_newPosition = right.read();
  if (l_newPosition != l_oldPosition) {
    l_oldPosition = l_newPosition;
//    Serial.println(l_newPosition);
  }
  if (r_newPosition != r_oldPosition) {
    r_oldPosition = r_newPosition;
  }
  
  
  
}


