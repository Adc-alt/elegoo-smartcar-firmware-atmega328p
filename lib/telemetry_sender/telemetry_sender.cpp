#include "telemetry_sender.h"

#include "../ir_sensor/ir_sensor.h"

TelemetrySender::TelemetrySender(Stream& out, Stream& inStream, unsigned long sendInterval)
    : out(out), inStream(inStream), sendInterval(sendInterval)
{
}

void TelemetrySender::trySend(TelemetryFrame& frame)
{
  // NO enviar telemetría si hay datos entrantes (comando esperando)
  // El comando tiene PRIORIDAD ABSOLUTA
  if (shouldInterrupt())
  {
    return;
  }

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
  // Construir JSON manualmente - MUCHO más eficiente en RAM
  // No crea objetos anidados en memoria, solo escribe directamente al stream
  // Esto evita el overhead de la estructura interna de ArduinoJson
  // IMPORTANTE: Verifica después de cada print() si hay comandos entrantes
  // Si hay comandos, INTERRUMPE el envío inmediatamente

  if (shouldInterrupt())
    return;
  out.print(F("{\"type\":\""));

  if (shouldInterrupt())
    return;
  out.print(type);

  if (shouldInterrupt())
    return;
  out.print(F("\",\"seq\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.seq);

  if (shouldInterrupt())
    return;
  out.print(F(",\"t_ms\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.t_ms);

  if (shouldInterrupt())
    return;
  out.print(F(",\"sensors\":{"));

  // Switch button
  if (shouldInterrupt())
    return;
  out.print(F("\"switch\":{\"pressed\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.sw_pressed ? F("true") : F("false"));

  if (shouldInterrupt())
    return;
  out.print(F(",\"count\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.sw_count);

  if (shouldInterrupt())
    return;
  out.print(F("},"));

  // HCSR04
  if (shouldInterrupt())
    return;
  out.print(F("\"hcsr04\":{\"distanceCm\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.hcsr04_distanceCm);

  if (shouldInterrupt())
    return;
  out.print(F(",\"valid\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.hcsr04_measurementValid ? F("true") : F("false"));

  if (shouldInterrupt())
    return;
  out.print(F("},"));

  // IrSensor
  if (shouldInterrupt())
    return;
  out.print(F("\"irSensor\":{"));

  if (frame.ir_data != nullptr)
  {
    if (shouldInterrupt())
      return;
    out.print(F("\"data\":\""));

    if (shouldInterrupt())
      return;
    out.print(frame.ir_data);

    if (shouldInterrupt())
      return;
    out.print(F("\","));
  }

  if (shouldInterrupt())
    return;
  out.print(F("\"new\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.ir_new ? F("true") : F("false"));

  if (shouldInterrupt())
    return;
  out.print(F(",\"raw\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.ir_raw);

  if (shouldInterrupt())
    return;
  out.print(F("},"));

  // Battery
  if (shouldInterrupt())
    return;
  out.print(F("\"battery\":{\"voltage\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.battery_voltage, 2);

  if (shouldInterrupt())
    return;
  out.print(F("},"));

  // Line Sensor
  if (shouldInterrupt())
    return;
  out.print(F("\"lineSensor\":{\"left\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.line_sensor_left);

  if (shouldInterrupt())
    return;
  out.print(F(",\"middle\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.line_sensor_middle);

  if (shouldInterrupt())
    return;
  out.print(F(",\"right\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.line_sensor_right);

  if (shouldInterrupt())
    return;
  out.print(F("},"));

  // MPU
  if (shouldInterrupt())
    return;
  out.print(F("\"mpuSensor\":{\"ax\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_ax, 2);

  if (shouldInterrupt())
    return;
  out.print(F(",\"ay\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_ay, 2);

  if (shouldInterrupt())
    return;
  out.print(F(",\"az\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_az, 2);

  if (shouldInterrupt())
    return;
  out.print(F(",\"gx\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_gx, 2);

  if (shouldInterrupt())
    return;
  out.print(F(",\"gy\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_gy, 2);

  if (shouldInterrupt())
    return;
  out.print(F(",\"gz\":"));

  if (shouldInterrupt())
    return;
  out.print(frame.mpu_gz, 2);

  if (shouldInterrupt())
    return;
  out.print(F("}"));

  if (shouldInterrupt())
    return;
  out.println(F("}}"));
}
