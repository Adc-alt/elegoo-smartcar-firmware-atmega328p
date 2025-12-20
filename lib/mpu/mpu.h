#pragma once

#include <Arduino.h>
// MPU6050 es un typedef, no una clase, así que necesitamos el include completo
// Pero lo mantenemos aquí porque es necesario para el puntero en la clase
#include "../telemetry_frame/telemetry_frame.h"


#include <MPU6050.h>

// 1. Estructuras y enums
// struct MPUData
// {
//   float accelX, accelY, accelZ; // Valores del acelerometro
//   float gyroX, gyroY, gyroZ;    // Valores del giroscopio
// };

// 2. Clase principal
class Mpu
{
public:
  // 2.1 Constructor
  explicit Mpu(MPU6050& mpu);

  // 2.2 Métodos públicos principales
  void begin();
  void update(TelemetryFrame& frame);

  // 2.3 Getters
  // MPUData getMPUData();

private:
  static const int samples =
    30; // Número de medidas para la calibración (reducido de 50 a 30 para ahorrar tiempo y RAM)

  // 2.5 Punteros a componentes
  MPU6050* mpu;

  // 2.6 Variables de lectura
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  // Variables no usadas - comentadas para ahorrar RAM
  // float angleX, angleY, angleZ = 0;
  // unsigned long lastUpdate;

  // 2.7 Variables de calibración
  float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
  float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

  // 2.8 Métodos privados
  void calibrate();
};
