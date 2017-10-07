/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

image: http://danielmartingonzalez.azurewebsites.net/content/images/2017/07/ejemplo-encender-apagar-led.png



  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

const int internalLed = 16; //shows up as D0 on ESP8266

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(16, OUTPUT);  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(16, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(16, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
