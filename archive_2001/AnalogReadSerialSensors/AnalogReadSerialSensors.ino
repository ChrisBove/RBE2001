/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(0);
  int sensorValue1 = analogRead(1);
  int sensorValue2 = analogRead(3);
  int sensorValue3 = analogRead(4);
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print( "\t");
  Serial.print(sensorValue1);
  Serial.print( "\t");
  Serial.print(sensorValue2);
  Serial.print( "\t");
  Serial.print(sensorValue3);
  Serial.println( "\t");
  delay(1);        // delay in between reads for stability
}
