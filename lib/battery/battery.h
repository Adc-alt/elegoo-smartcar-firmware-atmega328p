#pragma once

#include "../telemetry_state/telemetry_state.h"

#include <Arduino.h>

class Battery
{
public:
  // 3. Constructor
  // Todo constructor con un solo parametro debe ser explicit para evitar conversiones implicitas
  explicit Battery(uint8_t pinVolt);

  // 4. Métodos públicos principales
  void update(TelemetryState& state);
  void begin();

private:
  uint8_t pinVolt;
  BatteryStatus status = BatteryStatus::BatteryGood;

  unsigned long lastMeasureMs = 0;
  float voltage               = 0.0;

  float readVoltage();
  void updateStatus();
  // 2. Constantes de configuración
  // #define MEASURE_TIME 120000 // 2 minutos
  static constexpr unsigned long measureTimeMs = 1000;
  static constexpr float voltageThreshold      = 7.8;
  static constexpr float tolerance             = 0.08;
};