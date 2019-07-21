#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(10, 11); // (RX, TX)
Adafruit_MMA8451 mma = Adafruit_MMA8451(); // obje

bool isControlling = false;

void setup(void) {
  Serial.begin(57600);
  xbee.begin(9600);
  mma.begin();
  mma.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  // Read the 'raw' data in 14-bit counts
  mma.read();
  double sum = 0;

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  
  sum = (event.acceleration.x*event.acceleration.x) +
    (event.acceleration.y*event.acceleration.y) +
    (event.acceleration.z*event.acceleration.z);

  
  xbee.print(sum);
  xbee.println(",");
  

  if(sum >= 300) {
    isControlling = true;
  }

  if(isControlling) {
    if(sum >= 0 && sum <= 100) {
      xbee.println("DC motor is activated.");
      isControlling = false;
      delay(1000);
    }
  }
}
