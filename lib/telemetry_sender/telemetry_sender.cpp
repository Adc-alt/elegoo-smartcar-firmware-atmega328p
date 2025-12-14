#include "telemetry_sender.h"

#include "../ir_sensor/ir_sensor.h"

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
    state.t_ms = currentTime;
    send(state);
  }
}

void TelemetrySender::send(const TelemetryState& state)
{
  // Aumentado a 384 bytes para asegurar que cabe todo el JSON con battery
  StaticJsonDocument<384> doc;

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

  // IrSensor
  JsonObject irSensor = sensors.createNestedObject("irSensor");
  irSensor["mode"]    = statusToString(state.ir_mode);

  // Battery
  JsonObject battery = sensors.createNestedObject("battery");
  battery["voltage"] = state.battery_voltage;
  battery["status"]  = batteryStatusToString(state.battery_status);

  // Verificar si hay espacio suficiente
  if (doc.overflowed())
  {
    out.print(F("ERROR: JSON overflow"));
    out.println();
    return;
  }

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

const char* TelemetrySender::batteryStatusToString(BatteryStatus status)
{
  switch (status)
  {
    case BatteryStatus::BatteryGood:
      return "good";
    case BatteryStatus::BatteryEmergency:
      return "emergency";
    default:
      return "unknown";
  }
}