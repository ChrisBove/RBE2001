#include <TimerOne.h>

// steps a stepper motor about

#define stepPin 11
#define dirPin 10

// use 10000 microseconds, or 10ms
#define time 1000

bool dir = false; // false for one dir
int count = 1;
bool fast = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(13, OUTPUT);
  
  // initialize to off
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(13, LOW);
  
  Timer1.initialize(time);
  Timer1.attachInterrupt( timerISR );
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  fast = false;
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerISR()
{
  if(fast) {
    // Toggle LED
    digitalWrite( stepPin, digitalRead( stepPin ) ^ 1 );
  }
  else {
        // Toggle LED
    if (count == 0) {
      digitalWrite( stepPin, digitalRead( stepPin ) ^ 1 );
      count = 1;
    }
    else
      count = 0;
  }
//    digitalWrite( 13, digitalRead( 13 ) ^ 1 );
}
