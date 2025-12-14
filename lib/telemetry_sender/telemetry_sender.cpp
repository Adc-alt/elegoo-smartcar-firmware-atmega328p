#include "telemetry_sender.h"

#include "../ir_sensor/ir_sensor.h"

TelemetrySender::TelemetrySender(Stream& out, unsigned long sendInterval) : out(out), sendInterval(sendInterval)
{
}

void TelemetrySender::trySend(TelemetryFrame& frame)
{
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= sendInterval)
  {
    lastSendTime = currentTime;
    frame.seq++; // Incrementar secuencia solo cuando realmente enviamos
    frame.t_ms = currentTime;
    send(frame);
  }
}

void TelemetrySender::send(const TelemetryFrame& frame)
{
  // Aumentado a 384 bytes para asegurar que cabe todo el JSON con battery
  StaticJsonDocument<384> doc;

  doc["type"] = type;
  doc["seq"]  = frame.seq;
  doc["t_ms"] = frame.t_ms;

  JsonObject sensors = doc.createNestedObject("sensors");

  // Switch button
  JsonObject sw = sensors.createNestedObject("switch");
  sw["pressed"] = frame.sw_pressed;
  sw["count"]   = frame.sw_count;

  // HCSR04
  JsonObject hcsr04    = sensors.createNestedObject("hcsr04");
  hcsr04["distanceCm"] = frame.hcsr04_distanceCm;
  hcsr04["valid"]      = frame.hcsr04_measurementValid;

  // IrSensor
  JsonObject irSensor = sensors.createNestedObject("irSensor");
  if (frame.ir_data != nullptr)
  {
    irSensor["data"] = frame.ir_data;
  }
  irSensor["new"] = frame.ir_new;
  irSensor["raw"] = frame.ir_raw;

  // Battery
  JsonObject battery = sensors.createNestedObject("battery");
  battery["voltage"] = frame.battery_voltage;

  // Line Sensor
  JsonObject lineSensor = sensors.createNestedObject("lineSensor");
  lineSensor["left"]    = frame.line_sensor_left;
  lineSensor["middle"]  = frame.line_sensor_middle;
  lineSensor["right"]   = frame.line_sensor_right;

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
