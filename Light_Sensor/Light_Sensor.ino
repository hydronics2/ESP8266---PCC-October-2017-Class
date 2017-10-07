/*
  ReadAnalogVoltage

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage

  picture: http://osoyoo.com/wp-content/uploads/2016/12/ppppppp.png
*/


const int analogPin = A0; // Marked A0 on ESP8266


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(analogPin);
  // analog reading (which goes from 0 - 1023)

  // print out the value you read:
  Serial.println(sensorValue);
}
