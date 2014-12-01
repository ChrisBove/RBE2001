#include <TimerOne.h>

// steps a stepper motor about

#define stepPin 10
#define dirPin 11



void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  // initialize to off
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
}


void loop() {
  // put your main code here, to run repeatedly:

}
