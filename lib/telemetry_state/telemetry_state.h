#pragma once
#include <stdint.h>
/*
  SensorState
  ===========
  Este archivo define EL MODELO DE DATOS del sistema.

  - No hay lógica
  - No hay hardware
  - No hay JSON
  - Solo variables que representan el estado actual
*/

// --------------------
// Enums de estado
// --------------------

// Modo del botón (parte del estado global)
enum class SwitchButtonStatus : uint8_t
{
  Mode1,
  Mode2,
  Mode3,
  Mode4
};

// --------------------
// Estado global de sensores
// --------------------

struct TelemetryState
{
  // ---- Metadatos ----
  uint32_t t_ms = 0; // timestamp (millis)
  uint16_t seq  = 0; // contador de mensajes

  // ---- Switch Button ----
  bool sw_pressed            = false; // ¿está pulsado ahora?
  uint16_t sw_count          = 0;     // nº de pulsaciones
  SwitchButtonStatus sw_mode = SwitchButtonStatus::Mode1;

  // ---- HCSR04 ----
  uint16_t hcsr04_distanceCm   = 0;
  bool hcsr04_measurementValid = false;

  // ---- Sensores futuros (ejemplos) ----
  // float battery_v = 0.0f;
  // uint16_t line_l = 0;
  // uint16_t line_c = 0;
  // uint16_t line_r = 0;
  // float hcsr04_cm = 0.0f;
  // float mpu_ax = 0.0f;
  // float mpu_ay = 0.0f;
  // float mpu_az = 0.0f;
};
