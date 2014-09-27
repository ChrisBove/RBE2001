#include <Servo.h> 
 
int power2 = 24;
int power7 = 25; 
const int potPin = 10;

 // constants, but must be tuned! (start at 0)
 const float Kp = 0.0000006;
 const float Ki = 0.0000000006;
 const float Kd = 0.3;
 
 float lastError = 0; // stored from previous value.
 float errorSum = 0;  // sum of errors
 int count = 0;  // used to reset sum
 
void setup() 
{ 
  Serial.begin(9600);
  pinMode(power2, OUTPUT);   
  pinMode(power7,OUTPUT);
  digitalWrite(power2, LOW);
  digitalWrite(power7, LOW);
  
//  rotateRight();
} 
 
void sendOutput(float output) {
  if (abs(output) > 0.5) {
  if (output < 0) {
    
    rotateLeft();
  }
  else {
    
    rotateRight();
  }
  }
  else
  freeze();
}

void rotateLeft() {
  analogWrite(power7, 0);
  analogWrite(power2, 150);
}

void rotateRight() {
  analogWrite(power2, 0);
  analogWrite(power7, 150);
}

void freeze() {
  analogWrite(power2, 0);
  analogWrite(power7, 0);
}

void setPID(int setpoint) {
  int desPos = setpoint;  // desired input right pos, left negative
  int potPos = analogRead(potPin);  // sensor input
  
  int currentError = desPos - potPos; // calculate the current error
  
  // do output calculation - the main thing!
  float output = Kp*(currentError) + Ki*(errorSum) + Kd*(currentError-lastError);
  
  Serial.print("Output: ");
  Serial.println(output);
  delay(10); // prevent spamming
  
  sendOutput(output); // send output
  
  // increment errorSum if not too big
  if (count < 1000)
    errorSum += currentError;
  else
    errorSum = currentError;
    
  // set last error equal to this
  lastError = currentError;
}

void loop() 
{ 
  unsigned long int startTime = millis();
  while (millis() - startTime < 1000) {
    setPID(300);
  }
  
  startTime = millis();
  while(millis() - startTime < 1000) {
    setPID(150);
  }
  
//  int desPos = 300;  // desired input right pos, left negative
//  int potPos = analogRead(potPin);  // sensor input
//  
//  int currentError = desPos - potPos; // calculate the current error
//  
//  // do output calculation - the main thing!
//  float output = Kp*(currentError) + Ki*(errorSum) + Kd*(currentError-lastError);
//  
//  Serial.print("Output: ");
//  Serial.println(output);
//  delay(10); // prevent spamming
//  
//  sendOutput(output); // send output
//  
//  // increment errorSum if not too big
//  if (count < 1000)
//    errorSum += currentError;
//  else
//    errorSum = currentError;
//    
//  // set last error equal to this
//  lastError = currentError;
} 

