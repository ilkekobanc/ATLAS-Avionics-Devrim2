#include <SoftwareSerial.h>
#include <MPU9255.h>

#define g 9.81
#define magnetometer_cal 0.06

SoftwareSerial mySerial(10, 11); // RX, TX
MPU9255 mpu;

String aX;
String aY;
String aZ;

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
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  mpu.init();
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() {

  mpu.read_acc();
  
  aX = static_cast<String>(process_acceleration(mpu.ax, scale_2g));
  aY = static_cast<String>(process_acceleration(mpu.ay, scale_2g));
  aZ = static_cast<String>(process_acceleration(mpu.az, scale_2g));
  //mySerial.print(aX);
  mySerial.println(aY);
  //mySerial.println(aZ);
  delay(100);
}
