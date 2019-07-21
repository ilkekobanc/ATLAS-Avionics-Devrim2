#include <MPU9255.h>

#define g 9.81
#define magnetometer_cal 0.06

const int dc = 4;
bool isFirstTime = true;

MPU9255 mpu;

double aX;
double aY;
double aZ;

double gX;
double gY;
double gZ;

double mX;
double mY;
double mZ;

double process_acceleration(int input, scales sensor_scale) {
  /*
  To get acceleration in 'g', each reading has to be divided by :
   -> 16384 for +- 2g scale (default scale)
   -> 8192  for +- 4g scale
   -> 4096  for +- 8g scale
   -> 2048  for +- 16g scale
  */
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
  /*
  To get rotation velocity in dps (degrees per second), each reading has to be divided by :
   -> 131   for +- 250  dps scale (default value)
   -> 65.5  for +- 500  dps scale
   -> 32.8  for +- 1000 dps scale
   -> 16.4  for +- 2000 dps scale
  */

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

//process raw magnetometer data
//input = raw reading from the sensor, sensitivity =
//returns : magnetic flux density in μT (in micro Teslas)
double process_magnetic_flux(int16_t input, double sensitivity)
{
  /*
  To get magnetic flux density in μT, each reading has to be multiplied by sensitivity
  (Constant value different for each axis, stored in ROM), then multiplied by some number (calibration)
  and then divided by 0.6 .
  (Faced North each axis should output arround 31 µT without any metal / walls around
  Note : This manetometer has really low initial calibration tolerance : +- 500 LSB !!!
  Scale of the magnetometer is fixed -> +- 4800 μT.
  */
  return (input*magnetometer_cal*sensitivity)/0.6;
}

void setup() {
  Serial.begin(9600);
  if(mpu.init()) {
    Serial.println("failed!");
  } else {
    Serial.println("successful!");
  }
  pinMode(dc, OUTPUT);
}

void loop() {
  mpu.read_acc();
  mpu.read_gyro();
  mpu.read_mag();
  aX = process_acceleration(mpu.ax, scale_2g);
  if(aX < 7) {
    while(isFirstTime) {
      digitalWrite(dc, HIGH);
      delay(3000);
      digitalWrite(dc, LOW);
      isFirstTime = false;
    }
    Serial.println(aX);
  } else {
    digitalWrite(dc, LOW);
  }
  delay(100);
}
