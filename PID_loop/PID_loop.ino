#include <Servo.h> 
 
Servo motor;
 
void setup() 
{ 
  motor.attach(9); 
} 
 
 // constants, but must be tuned! (start at 0)
 const float Kp = 0.0;
 const float Ki = 0.0;
 const float Ka = 0.0;
 
 float lastError = 0; // stored from previous value.
 float errorSum = 0;  // sum of errors
 int count = 0;  // used to reset sum
void loop() 
{ 
  int desPos = 38;  // desired input
  int potPos = analogRead(0);  // sensor input
  
  int currentError = desPos - potPos; // calculate the current error
  
  // do output calculation - the main thing!
  float output = Kp*(currentError) + Ki*(errorSum) + Ka*(currentError-lastError);
  
  motor.write(output); // send output
  
  // increment errorSum if not too big
  if (count < 1000)
    errorSum += currentError;
  else
    errorSum = currentError;
    
  // set last error equal to this
  lastError = currentError;
  
} 

