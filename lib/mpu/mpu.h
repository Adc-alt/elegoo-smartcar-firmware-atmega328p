#pragma once

#include <Arduino.h>
// MPU6050 es un typedef, no una clase, así que necesitamos el include completo
// Pero lo mantenemos aquí porque es necesario para el puntero en la clase

#include <MPU6050.h>

// 1. Estructuras y enums

// 2. Clase principal
class Mpu
{
public:
  enum MpuIndex
  {
    ACCEL_X = 0,
    ACCEL_Y = 1,
    ACCEL_Z = 2,
    GYRO_X  = 3,
    GYRO_Y  = 4,
    GYRO_Z  = 5
  };
  // 2.1 Constructor
  explicit Mpu(MPU6050& mpu);

  // 2.2 Métodos públicos principales
  void begin();
  void getMpuData();

  float getValue(MpuIndex index) const
  {
    return data[index];
  }

private:
  float data[6];
  static const int samples =
    30; // Número de medidas para la calibración (reducido de 50 a 30 para ahorrar tiempo y RAM)

  // 2.5 Punteros a componentes
  MPU6050* mpu;

  // Variables no usadas - comentadas para ahorrar RAM
  // float angleX, angleY, angleZ = 0;
  // unsigned long lastUpdate;

  // 2.7 Variables de calibración
  float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
  float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

  // 2.8 Métodos privados
  void calibrate();
};
