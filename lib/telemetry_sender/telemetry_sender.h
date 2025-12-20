#pragma once
#include "../telemetry_frame/telemetry_frame.h"

#include <Arduino.h>
#include <ArduinoJson.h>

// Esta clase SOLO se encarga de:
// TelemetryFrame  --->  JSON  --->  Serial
// Maneja internamente el timing de envío
class TelemetrySender
{
public:
  // Le pasas el Stream por el que quieres enviar (Serial, Serial1, etc.)
  // y el intervalo de envío en milisegundos
  explicit TelemetrySender(Stream& out, unsigned long sendInterval);

  // Intenta enviar telemetría (solo envía si ha pasado el intervalo)
  // Incrementa frame.seq solo si realmente envía
  void trySend(TelemetryFrame& frame);

  // Enviar una "foto" del estado de sensores (sin control de timing)
  // Usar solo si necesitas forzar un envío inmediato
  void send(const TelemetryFrame& frame);

private:
  Stream& out;
  const char* type           = "telemetry_frame";
  unsigned long lastSendTime = 0;
  unsigned long sendInterval;
};
