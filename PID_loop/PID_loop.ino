#include <Servo.h> 
 
int power2 = 13;
int power7 = 12; 
const int potPin = 10;

 // constants, but must be tuned! (start at 0)
 const float Kp = 0.00001; //0.0016; //.006;
 const float Ki = 0.00000005; //0.00001; //0.00016; //.0000000006;
 const float Kd = 0.0001;
 
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
  if (abs(output) > 0) {
  if (output < 0) {
    
    rotateLeft(scaleOutput(output));
  }
  else {
    
    rotateRight(scaleOutput(output));
  }
  }
  else
  freeze();
}

float scaleOutput (float output) {
  float result = 65+abs(255*output);
  Serial.print("scaled: ");
  Serial.print(result);  
  Serial.println("\t"); 
  return result;
}

void rotateLeft(float output) {
  analogWrite(power7, 0);
  analogWrite(power2, output);
}

void rotateRight(float output) {
  analogWrite(power2, 0);
  analogWrite(power7, output);
}

void freeze() {
  analogWrite(power2, 0);
  analogWrite(power7, 0);
}

int analogToDegrees(float analogVal) {
  // have 1024 ticks to 270 degrees
  return (analogVal*0.263671875); // that's degrees 0-270
}

void setPID(int setpointDegrees) {
  int desPos = setpointDegrees;  // desired input right pos, left negative
  int curPos = analogToDegrees(analogRead(potPin));  // sensor input
  
  int currentError = desPos - curPos; // calculate the current error
  Serial.print("currentError: ");
  Serial.print(currentError);
  Serial.print("\t");  
  // do output calculation - the main thing!
  float output = Kp*(currentError) + Ki*(errorSum) + Kd*(currentError-lastError);
  
  Serial.print("Output: ");
  Serial.print(output);
  Serial.print("\t"); 
  delay(10); // prevent spamming
  
  if (abs(currentError) > 2)
    sendOutput(output); // send output
  else
    sendOutput(0);
  // increment errorSum if not too big
  if (count < 100) {
    errorSum += currentError;
    count = 0;
  }
  else
    errorSum = currentError;
  count ++;
  Serial.print("errorSum: ");
  Serial.print(errorSum);  
  Serial.print("\t"); 
    
  // set last error equal to this
  lastError = currentError;
}

void loop() 
{ 
  setPID(127);
//  unsigned long int startTime = millis();
//  while (millis() - startTime < 4000) {
////    Serial.println(analogRead(10));
//    setPID(450);
//  }
//  
//  errorSum = 0;
//  lastError = 0;
//  count = 0;
//  
//  startTime = millis();
//  while(millis() - startTime < 4000) {
////    Serial.println(analogRead(10));
//    setPID(100);
//  }
  
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

