/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int power2 = 22;
int power7 = 23; 

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(power2, OUTPUT);   
  pinMode(power7,OUTPUT);
  digitalWrite(power2, LOW);
  digitalWrite(power7, LOW);  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(power2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(power2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
