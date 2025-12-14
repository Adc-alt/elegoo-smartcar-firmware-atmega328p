#pragma once

#include "../telemetry_frame/telemetry_frame.h"

#include <Arduino.h>

class Battery
{
public:
  // 3. Constructor
  // Todo constructor con un solo parametro debe ser explicit para evitar conversiones implicitas
  explicit Battery(uint8_t pinVolt);

  // 4. Métodos públicos principales
  void update(TelemetryFrame& frame);
  void begin();

private:
  uint8_t pinVolt;

  unsigned long lastMeasureMs = 0;
  float voltage               = 0.0;

  float readVoltage();
  // 2. Constantes de configuración
  // #define MEASURE_TIME 120000 // 2 minutos
  static constexpr unsigned long measureTimeMs = 1000;
  static constexpr float voltageThreshold      = 7.8;
  static constexpr float tolerance             = 0.08;
};