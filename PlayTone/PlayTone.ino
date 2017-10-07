/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8
  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"

int audioPin = 4; //D2 on ESP8266

void setup() {

  Serial.begin(115200);
}

int frequencyPlaying = 200;

void loop() {

  Serial.println(frequencyPlaying);
  tone(audioPin, frequencyPlaying, 500);
  delay(100);

  if(frequencyPlaying < 6000){
    frequencyPlaying = frequencyPlaying + 100;
  }else{
    frequencyPlaying = 200;
  }

}

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration) {
  pinMode (_pin, OUTPUT );
  analogWriteFreq(frequency);
  analogWrite(_pin,500);
  delay(duration);
  analogWrite(_pin,0);
}
