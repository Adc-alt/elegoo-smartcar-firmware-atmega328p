#pragma once
#include <stdint.h>
/*
  TelemetryFrame
  ===========
  Lo he querido llamar así porque:
  Se genera periódicamente o como snapshot coherente

    -  Representa un instante de tiempo
    -  Tiene timestamp y seq (clave)
    -  Es consumido por otro sistema
    -  No implica intención ni interpretación
    -  Un frame es una unidad de datos en un stream.

  Este archivo define EL MODELO DE DATOS del sistema.

  - No hay lógica
  - No hay hardware
  - No hay JSON
  - Solo variables que representan el estado actual
*/

struct TelemetryFrame
{
  // ---- Metadatos ----
  uint32_t t_ms = 0; // timestamp (millis)
  uint16_t seq  = 0; // contador de mensajes

  // ---- Switch Button ----
  bool sw_pressed   = false; // ¿está pulsado ahora?
  uint16_t sw_count = 0;     // nº de pulsaciones

  // ---- HCSR04 ----
  uint16_t hcsr04_distanceCm   = 0;
  bool hcsr04_measurementValid = false;

  // ---- IrSensor ----
  bool ir_new         = false;
  uint32_t ir_raw     = 0;
  const char* ir_data = nullptr; // String del comando IR ("forward", "backward", etc.)

  // ---- Battery ----
  float battery_voltage = 0.0f;

  // ---- Line Sensor ----
  uint16_t line_sensor_left   = 0;
  uint16_t line_sensor_middle = 0;
  uint16_t line_sensor_right  = 0;

  // ---- MPU ----
  float mpu_ax = 0.0f;
  float mpu_ay = 0.0f;
  float mpu_az = 0.0f;
  float mpu_gx = 0.0f;
  float mpu_gy = 0.0f;
  float mpu_gz = 0.0f;

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
