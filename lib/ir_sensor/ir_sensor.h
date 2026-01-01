#pragma once

#include "../telemetry_frame/telemetry_frame.h"

#include <Arduino.h>

// Forward declaration para evitar múltiples definiciones

// ===================== DEFINICIÓN DE CONSTANTES =====================
static constexpr unsigned long IR_TIMEOUT_MS = 100;

// ===================== INCLUDES =====================
// class IRrecv;

class IrSensor
{
public:
  explicit IrSensor(uint8_t pinIR);

  void begin();
  const char* getIrCommand();

private:
  uint8_t pinIR;
  unsigned long lastIRTime = 0;
  bool irNew               = false;
  unsigned long irRaw      = 0;
  const char* irCommand    = "stop";
};
