/*
  This code implements a PID control to drive a linkage to a precoded angle.
  
  Authors: Christopher Bove, Christopher Ellen, Sami Neeno
  Last modified: 9/30/14
  
*/

// **** INCLUDE LIBRARIES ******
#include <Servo.h> 

// define pin constants
const int hBridgePin2 = 13;
const int hBridgePin7 = 12; 
const int potPin = 10;

 // constants for PID, casually tuned for system
 const float Kp = 0.00001;
 const float Ki = 0.00000005; 
 const float Kd = 0.0001;
 
 // ****** GLOBALS ********
 float lastError = 0; // stored from previous value.
 float errorSum = 0;  // sum of errors
 int count = 0;  // used to reset sum
 
// setup
void setup() 
{ 
  Serial.begin(9600);        // begin serial comm
  pinMode(hBridgePin2, OUTPUT);   // set pins to output
  pinMode(hBridgePin7,OUTPUT);
  digitalWrite(hBridgePin2, LOW);  // immediately set low to prevent shorts
  digitalWrite(hBridgePin7, LOW);
} 

// takes output from PWM and sends it through to the motors
void sendOutput(float output) {
  // if output is not 0
  if (abs(output) > 0) {
    // if output is negative, rotate left
    if (output < 0) {
      rotateLeft(scaleOutput(output));
    }
    else {
      // otherwise, go right
      rotateRight(scaleOutput(output));
    }
  }
  // otherwise, we are perfect, so stop the motor
  else
    freeze();
}

// scales a passed output to prevent motor stalls using PWM and analog write
float scaleOutput (float output) {
  // adds a bit to prevent stall, scales to 255
  float result = 65+abs(255*output); // if value is over 255, analog write doesn't care
  Serial.print("scaled: "); // print for debug
  Serial.print(result);  
  Serial.println("\t"); 
  return result;
}

// takes output and configures hbridge to rotate motor left using analogWrite
void rotateLeft(float output) {
  analogWrite(hBridgePin7, 0);      // ground one control line
  analogWrite(hBridgePin2, output); // give PWM output
}

// takes output and configures hbridge to rotate motor left using analogWrite
void rotateRight(float output) {
  analogWrite(hBridgePin2, 0);      // ground one control line
  analogWrite(hBridgePin7, output); // give PWM output
}

// grounds both pins to hbridge to stop power (does not hard brake)
void freeze() {
  analogWrite(hBridgePin2, 0);
  analogWrite(hBridgePin7, 0);
}

// converts analog pot value to degrees 
int analogToDegrees(float analogVal) {
  // have 1024 ticks to 270 degrees
  return (analogVal*0.263671875); // that's degrees 0-270
}

// main PID loop - takes setpoint in degrees and drives motor appropriately
void setPID(int setpointDegrees) {
  int desPos = setpointDegrees;  // desired input: right is +, left - negative
  int curPos = analogToDegrees(analogRead(potPin));  // sensor input to degrees
  
  int currentError = desPos - curPos; // calculate the current error
  Serial.print("currentError: "); // debug print
  Serial.print(currentError);
  Serial.print("\t");  
  
  //////// do output calculation - the main thing! /////////
  float output = Kp*(currentError) + Ki*(errorSum) + Kd*(currentError-lastError);
  
  Serial.print("Output: "); // debug print
  Serial.print(output);
  Serial.print("\t"); 
  delay(10); // prevent spamming serial
  
  // if error is beyond 2 degrees, send new output
  if (abs(currentError) > 2)
    sendOutput(output); // send output
  else
    sendOutput(0); // otherwise, set to stop
    
  // increment errorSum if not too big/long
  if (count < 100) {
    errorSum += currentError; // add error to sum
    count++; // increment count
  }
  else {
    errorSum = currentError; // reset error sum to current
    count = 0; // reset count
  }
  Serial.print("errorSum: "); // debug
  Serial.print(errorSum);  
  Serial.print("\t"); 
    
  // set last error equal to this error
  lastError = currentError;
}

// main loop
void loop() 
{ 
  setPID(127); // set fixed setpoint
  
  // the code below was used to test having the PID control move from one
  // position/setpoint to the other over a few seconds
  
  unsigned long int startTime = millis();
  while (millis() - startTime < 4000) {
    setPID(450);
  }
  
  errorSum = 0;
  lastError = 0;
  count = 0;
  
  startTime = millis();
  while(millis() - startTime < 4000) {
    setPID(100);
  }
  
  errorSum = 0;
  lastError = 0;
  count = 0;
}
