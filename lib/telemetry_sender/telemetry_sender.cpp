#include "telemetry_sender.h"

TelemetrySender::TelemetrySender(Stream& out, unsigned long sendInterval) : out(out), sendInterval(sendInterval)
{
}

void TelemetrySender::trySend(TelemetryState& state)
{
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= sendInterval)
  {
    lastSendTime = currentTime;
    state.seq++; // Incrementar secuencia solo cuando realmente enviamos
    send(state);
  }
}

void TelemetrySender::send(const TelemetryState& state)
{
  StaticJsonDocument<384> doc; // Aumentado para incluir HCSR04

  doc["type"] = type;
  doc["seq"]  = state.seq;
  doc["t_ms"] = state.t_ms;

  JsonObject sensors = doc.createNestedObject("sensors");

  // Switch button
  JsonObject sw = sensors.createNestedObject("switch");
  sw["pressed"] = state.sw_pressed;
  sw["count"]   = state.sw_count;
  sw["mode"]    = switchModeToString(state.sw_mode);

  // HCSR04
  JsonObject hcsr04    = sensors.createNestedObject("hcsr04");
  hcsr04["distanceCm"] = state.hcsr04_distanceCm;
  hcsr04["valid"]      = state.hcsr04_measurementValid;

  serializeJson(doc, out);
  out.println();
}

const char* TelemetrySender::switchModeToString(SwitchButtonStatus mode)
{
  switch (mode)
  {
    case SwitchButtonStatus::Mode1:
      return "Mode1";
    case SwitchButtonStatus::Mode2:
      return "Mode2";
    case SwitchButtonStatus::Mode3:
      return "Mode3";
    case SwitchButtonStatus::Mode4:
      return "Mode4";
    default:
      return "Unknown";
  }
}