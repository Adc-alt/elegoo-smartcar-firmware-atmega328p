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
  // inStream es el mismo Stream pero para verificar si hay datos entrantes (comandos)
  explicit TelemetrySender(Stream& out, Stream& inStream, unsigned long sendInterval);

  // Intenta enviar telemetría (solo envía si ha pasado el intervalo)
  // Incrementa frame.seq solo si realmente envía
  // El envío se INTERRUMPE automáticamente si llega un comando
  void trySend(TelemetryFrame& frame);

  // Enviar una "foto" del estado de sensores (sin control de timing)
  // Usar solo si necesitas forzar un envío inmediato
  // El envío se INTERRUMPE automáticamente si llega un comando
  void send(const TelemetryFrame& frame);

private:
  Stream& out;
  Stream& inStream; // Para verificar si hay comandos entrantes
  const char* type           = "telemetry_frame";
  unsigned long lastSendTime = 0;
  unsigned long sendInterval;

  // Helper para verificar si debe interrumpirse el envío
  bool shouldInterrupt() const
  {
    return inStream.available() > 0;
  }
};
