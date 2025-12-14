#pragma once
#include "../telemetry_state/telemetry_state.h"

#include <Arduino.h>
/*
  Clase Hcsr04
  ============
  Representa UN sensor de ultrasonidos.

  Responsabilidad:
  - Leer la distancia
  - Guardar el último valor
  - Escribir ese valor en TelemetryState

  NO:
  - NO imprime por Serial
  - NO crea JSON
  - NO decide qué hacer con la distancia
*/

class Hcsr04
{
public:
  Hcsr04(uint8_t trigPin, uint8_t echoPin);

  void begin();
  void update(TelemetryState& state); // escribe en el estado común

private:
  uint8_t trigPin;
  uint8_t echoPin;

  // Momento en el que se hizo la última medición
  uint32_t lastScanTimeMs = 0;

  // Última distancia medida (en centímetros)
  uint16_t lastDistanceCm = 0;

  // ¿La medición fue válida?
  bool lastMeasurementValid = false;

  uint16_t measureDistanceCm(bool& valid);
};
