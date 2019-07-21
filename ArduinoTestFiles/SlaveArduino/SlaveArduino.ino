#include <Wire.h>

String message;

void setup() {
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  message = Wire.read();
  Serial.println(message);
}

void loop() {
  
}
