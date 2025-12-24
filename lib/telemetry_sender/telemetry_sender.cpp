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
  // Construir JSON manualmente - MUCHO más eficiente en RAM
  // No crea objetos anidados en memoria, solo escribe directamente al stream
  // Esto evita el overhead de la estructura interna de ArduinoJson

  out.print(F("{\"type\":\""));
  out.print(type);
  out.print(F("\",\"seq\":"));
  out.print(frame.seq);
  out.print(F(",\"t_ms\":"));
  out.print(frame.t_ms);
  out.print(F(",\"sensors\":{"));

  // Switch button
  out.print(F("\"switch\":{\"pressed\":"));
  out.print(frame.sw_pressed ? F("true") : F("false"));
  out.print(F(",\"count\":"));
  out.print(frame.sw_count);
  out.print(F("},"));

  // HCSR04
  out.print(F("\"hcsr04\":{\"distanceCm\":"));
  out.print(frame.hcsr04_distanceCm);
  out.print(F(",\"valid\":"));
  out.print(frame.hcsr04_measurementValid ? F("true") : F("false"));
  out.print(F("},"));

  // IrSensor
  out.print(F("\"irSensor\":{"));
  if (frame.ir_data != nullptr)
  {
    out.print(F("\"data\":\""));
    out.print(frame.ir_data);
    out.print(F("\","));
  }
  out.print(F("\"new\":"));
  out.print(frame.ir_new ? F("true") : F("false"));
  out.print(F(",\"raw\":"));
  out.print(frame.ir_raw);
  out.print(F("},"));

  // Battery
  out.print(F("\"battery\":{\"voltage\":"));
  out.print(frame.battery_voltage, 2);
  out.print(F("},"));

  // Line Sensor
  out.print(F("\"lineSensor\":{\"left\":"));
  out.print(frame.line_sensor_left);
  out.print(F(",\"middle\":"));
  out.print(frame.line_sensor_middle);
  out.print(F(",\"right\":"));
  out.print(frame.line_sensor_right);
  out.print(F("},"));

  // MPU
  out.print(F("\"mpuSensor\":{\"ax\":"));
  out.print(frame.mpu_ax, 2);
  out.print(F(",\"ay\":"));
  out.print(frame.mpu_ay, 2);
  out.print(F(",\"az\":"));
  out.print(frame.mpu_az, 2);
  out.print(F(",\"gx\":"));
  out.print(frame.mpu_gx, 2);
  out.print(F(",\"gy\":"));
  out.print(frame.mpu_gy, 2);
  out.print(F(",\"gz\":"));
  out.print(frame.mpu_gz, 2);
  out.print(F("}"));

  out.println(F("}}"));
}
