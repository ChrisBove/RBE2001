#include <TimerOne.h>

// steps a stepper motor about

#define stepPin 11
#define dirPin 10

// use 10000 microseconds, or 10ms
#define time 10000

bool dir = false; // false for one dir

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  
  // initialize to off
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  
  Timer1.initialize(time);
  Timer1.attachInterrupt( timerISR );
}


void loop() {
  // put your main code here, to run repeatedly:
  
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerISR()
{
    // Toggle LED
    digitalWrite( stepPin, digitalRead( stepPin ) ^ 1 );
}
