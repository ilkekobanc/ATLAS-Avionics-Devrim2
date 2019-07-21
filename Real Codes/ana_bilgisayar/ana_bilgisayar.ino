#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LPS.h>

static const int GPS_RXPin = 3, GPS_TXPin = 4;
static const int Xbee_RXPin = 11, Xbee_TXPin = 10;
static const uint32_t GPSBaud = 9600;
static const uint32_t XBEEBaud = 9600;

Adafruit_MMA8451 mma = Adafruit_MMA8451();
TinyGPSPlus gps;
SoftwareSerial gps_serial(GPS_RXPin, GPS_TXPin); // (4, 3)
SoftwareSerial xbee_t1(Xbee_RXPin, Xbee_TXPin); // (10, 11)
LPS ps;

int package_counter = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  gps_serial.begin(GPSBaud);
  xbee_t1.begin(XBEEBaud);
  ps.init();
  ps.enableDefault();
  mma.begin();
  mma.setRange(MMA8451_RANGE_2_G);
}

void loop() {

  package_counter++;

  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);

  gps_serial.listen();
  xbee_t1.print("?");
  xbee_t1.print(pressure);
  xbee_t1.print("\t");
  xbee_t1.print(altitude);
  xbee_t1.print("\t");
  xbee_t1.print(temperature);
  xbee_t1.print("\t");
  xbee_t1.print(gps.location.lat(), 10);
  xbee_t1.print("\t");
  xbee_t1.print(gps.location.lng(), 10);
  xbee_t1.print("\t");
  xbee_t1.print(gps.altitude.meters());
  xbee_t1.print("\t");
  xbee_t1.print(gps.speed.kmph());
  xbee_t1.print("\t");
  xbee_t1.print(event.acceleration.x);
  xbee_t1.print("\t");
  xbee_t1.print(event.acceleration.y);
  xbee_t1.print("\t");
  xbee_t1.println(event.acceleration.z);
  xbee_t1.print('!');
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
  
