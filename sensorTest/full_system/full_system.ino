#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <MPU9255.h>

#define g 9.81
#define magnetometer_cal 0.06

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
 
TinyGPSPlus gps;
MPU9255 mpu;
SoftwareSerial ss(RXPin, TXPin);

const char empty[] = "    ";

double process_acceleration(int input, scales sensor_scale) {
  
  double output = 1;

  // for +- 2g
  if(sensor_scale == scale_2g) {
    output = input;
    output = output / 16384;
    output = output * g;
  }

  //for +- 4g
  if(sensor_scale == scale_4g)
  {
    output = input;
    output = output/8192;
    output = output*g;
  }

  //for +- 8g
  if(sensor_scale == scale_8g)
  {
    output = input;
    output = output/4096;
    output = output*g;
  }

  //for +-16g
  if(sensor_scale == scale_16g)
  {
    output = input;
    output = output/2048;
    output = output*g;
  }

  return output;
}

double process_angular_velocity(int16_t input, scales sensor_scale )
{

  //for +- 250 dps
  if(sensor_scale == scale_250dps)
  {
    return input/131;
  }

  //for +- 500 dps
  if(sensor_scale == scale_500dps)
  {
    return input/65.5;
  }

  //for +- 1000 dps
  if(sensor_scale == scale_1000dps)
  {
    return input/32.8;
  }

  //for +- 2000 dps
  if(sensor_scale == scale_2000dps)
  {
    return input/16.4;
  }

  return 0;
}

double process_magnetic_flux(int16_t input, double sensitivity)
{
  return (input*magnetometer_cal*sensitivity)/0.6;
}

void setup() {

  Serial.begin(115200);
  ss.begin(GPSBaud);
  mpu.init();
}

void loop() {

  mpu.read_acc();
  mpu.read_gyro();
  mpu.read_mag();

  Serial.print(gps.location.lat());
  Serial.print(empty);
  Serial.print(gps.location.lng());
  Serial.print(empty);
  Serial.print(gps.speed.kmph());
  Serial.print(empty);
  Serial.print(process_acceleration(mpu.ax,scale_2g));
  Serial.print(empty);
  Serial.print(process_acceleration(mpu.ay,scale_2g));
  Serial.print(empty);
  Serial.print(process_acceleration(mpu.az,scale_2g));
  Serial.print(empty);
  Serial.print(process_angular_velocity(mpu.gx,scale_250dps));
  Serial.print(empty);
  Serial.print(process_angular_velocity(mpu.gy,scale_250dps));
  Serial.print(empty);
  Serial.print(process_angular_velocity(mpu.gz,scale_250dps));
  Serial.print(empty);
  Serial.print(process_magnetic_flux(mpu.mx,mpu.mx_sensitivity));
  Serial.print(empty);
  Serial.print(process_magnetic_flux(mpu.my,mpu.my_sensitivity));
  Serial.print(empty);
  Serial.print(process_magnetic_flux(mpu.mz,mpu.mz_sensitivity));
  Serial.print(empty);
  Serial.println();
  
  smartDelay(1000);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
