#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "../telemetry_state/telemetry_state.h"

#include <Arduino.h>

// Forward declaration para evitar múltiples definiciones

// ===================== DEFINICIÓN DE CONSTANTES =====================
static constexpr unsigned long IR_TIMEOUT_MS = 150;

// ===================== INCLUDES =====================
// class IRrecv;

class IrSensor
{
public:
  explicit IrSensor(uint8_t pinIR);

  void begin();
  void update(TelemetryState& state);

private:
  uint8_t pinIR;
  IrSensorStatus status    = IrSensorStatus::irSensorStop;
  unsigned long lastIRTime = 0;
};

// Mejor sin String en AVR:
const char* statusToString(IrSensorStatus status);

#endif