#pragma once
#include "../telemetry_state/telemetry_state.h"

#include <Arduino.h>
#include <ArduinoJson.h>

// Esta clase SOLO se encarga de:
// TelemetryState  --->  JSON  --->  Serial
// Maneja internamente el timing de envío
class TelemetrySender
{
public:
  // Le pasas el Stream por el que quieres enviar (Serial, Serial1, etc.)
  // y el intervalo de envío en milisegundos
  explicit TelemetrySender(Stream& out, unsigned long sendInterval);

  // Intenta enviar telemetría (solo envía si ha pasado el intervalo)
  // Incrementa state.seq solo si realmente envía
  void trySend(TelemetryState& state);

  // Enviar una "foto" del estado de sensores (sin control de timing)
  // Usar solo si necesitas forzar un envío inmediato
  void send(const TelemetryState& state);

private:
  Stream& out;
  const char* type           = "telemetry_state";
  unsigned long lastSendTime = 0;
  unsigned long sendInterval;

  // Helpers internos (no visibles fuera)
  static const char* switchModeToString(SwitchButtonStatus mode);
};
