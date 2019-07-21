#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int GPS_RXPin = 4, GPS_TXPin = 3;
static const int Xbee_RXPin = 11, Xbee_TXPin = 10;
static const uint32_t GPSBaud = 9600;
static const uint32_t XBEEBaud = 9600;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

TinyGPSPlus gps;

SoftwareSerial gps_serial(GPS_RXPin, GPS_TXPin); // (4, 3)
SoftwareSerial xbee_serial(Xbee_RXPin, Xbee_TXPin); // (10, 11)

int package_counter = 0;

void setup() {
  Serial.begin(115200);
  gps_serial.begin(GPSBaud);
  xbee_serial.begin(XBEEBaud);

  if (! mma.begin()) {
    Serial.println("Couldn't start");
    while (1);
  }
  Serial.println("MMA8451 found!");


  mma.setRange(MMA8451_RANGE_2_G);

}

void loop() {

  package_counter++;
  
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);

  gps_serial.listen();
  xbee_serial.print("?");
  xbee_serial.print("Zaman");
  xbee_serial.print(',');
  xbee_serial.print("team_id");
  xbee_serial.print(',');
  xbee_serial.print(package_counter);
  xbee_serial.print(',');
  xbee_serial.print(gps.location.lat(), 10);
  xbee_serial.print(',');
  xbee_serial.print(gps.location.lng(), 10);
  xbee_serial.print(',');
  xbee_serial.print(gps.altitude.meters());
  xbee_serial.print(',');
  xbee_serial.print("status");
  xbee_serial.println();
  xbee_serial.print('!');
  smartDelay(100);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gps_serial.available())
      gps.encode(gps_serial.read());
  } while (millis() - start < ms);
}
  
