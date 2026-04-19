#pragma once

#include <Arduino.h>

class Battery
{
public:
  // 3. Constructor
  // Cualquier constructor de un solo parámetro debe ser explicit para evitar conversiones implícitas
  explicit Battery(uint8_t pinVolt);

  // 4. Métodos públicos principales
  void begin();
  float readVoltage();
  float getVoltage();

private:
  uint8_t pinVolt;

  float voltage               = 0.0;
  unsigned long lastMeasureMs = 0;

  // float readVoltage();
  void updateStatus();
  // 2. Constantes de configuración
  // #define MEASURE_TIME 120000 // 2 minutos
  static constexpr unsigned long measureTimeMs = 12000;
  static constexpr float voltageThreshold      = 7.8;
  static constexpr float tolerance             = 0.08;
};