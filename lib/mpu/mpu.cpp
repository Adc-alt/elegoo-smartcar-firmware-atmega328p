#include "mpu.h"
// Wire.h solo se necesita aquí para las operaciones I2C
#include <Wire.h> // Biblioteca para comunicación I2C

// 1. Constructor
Mpu::Mpu(MPU6050& mpu)
{
  this->mpu = &mpu;
}

void Mpu::begin()
{
  Serial.println(F("[Mpu] Calibrando sensor, por favor no mueva el coche..."));
  calibrate();
  Serial.println(F("[Mpu] Sensor listo"));
}

void Mpu::update(TelemetryFrame& frame)
{
  int16_t ax, ay, az, gx, gy, gz;
  mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Normalizar y aplicar offset
  this->accelX = (ax / 16384.0) - this->accelOffsetX;
  this->accelY = (ay / 16384.0) - this->accelOffsetY;
  this->accelZ = (az / 16384.0) - this->accelOffsetZ;

  this->gyroX = (gx / 131.0) - this->gyroOffsetX;
  this->gyroY = (gy / 131.0) - this->gyroOffsetY;
  this->gyroZ = (gz / 131.0) - this->gyroOffsetZ;

  frame.mpu_ax = this->accelX;
  frame.mpu_ay = this->accelY;
  frame.mpu_az = this->accelZ;
  frame.mpu_gx = this->gyroX;
  frame.mpu_gy = this->gyroY;
  frame.mpu_gz = this->gyroZ;

  // Serial.println("=== MPU6050 Readings ===");
  // Serial.println("Accelerometer (g):");
  // Serial.print("X: ");
  // Serial.print(this->accelX, 3);
  // Serial.print(" Y: ");
  // // Logging de valores
  // Serial.print(this->accelY, 3);
  // Serial.print(" Z: ");
  // Serial.println(this->accelZ, 3);

  // Serial.println("Gyroscope (deg/s):");
  // Serial.print("X: ");
  // Serial.print(this->gyroX, 3);
  // Serial.print(" Y: ");
  // Serial.print(this->gyroY, 3);
  // Serial.print(" Z: ");
  // Serial.println(this->gyroZ, 3);
  // Serial.println("=====================");
}

void Mpu::calibrate()
{
  long axSum = 0, aySum = 0, azSum = 0;
  long gxSum = 0, gySum = 0, gzSum = 0;

  for (int i = 0; i < samples; i++)
  {
    int16_t ax, ay, az, gx, gy, gz;
    mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    axSum += ax;
    aySum += ay;
    azSum += az;
    gxSum += gx;
    gySum += gy;
    gzSum += gz;
    delay(10);
  }
  this->accelOffsetX = (axSum / (float)samples) / 16384.0;
  this->accelOffsetY = (aySum / (float)samples) / 16384.0;
  this->accelOffsetZ = ((azSum / (float)samples) / 16384.0) - 1.0; // Para Z, restar 1g (gravedad)

  // Calibración del giroscopio: restar el valor medio en reposo
  this->gyroOffsetX = (gxSum / (float)samples) / 131.0;
  this->gyroOffsetY = (gySum / (float)samples) / 131.0;
  this->gyroOffsetZ = (gzSum / (float)samples) / 131.0;
}

// MPUData Mpu::getMPUData()
// {
//   MPUData data;
//   data.accelX = this->accelX;
//   data.accelY = this->accelY;
//   data.accelZ = this->accelZ;
//   data.gyroX  = this->gyroX;
//   data.gyroY  = this->gyroY;
//   data.gyroZ  = this->gyroZ;
//   return data;
// }
