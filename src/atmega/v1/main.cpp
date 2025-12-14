#include <Arduino.h>
#include <ArduinoJson.h>
#include <battery.h>
#include <elegoo_smart_car_lib.h>
#include <hcsr04.h>
#include <ir_sensor.h>
#include <line_sensor.h>
#include <switch_button.h>
#include <telemetry_frame.h>
#include <telemetry_sender.h>

// Definiciones
#define INTERVAL 100

// Objetos
TelemetryFrame telemetryFrame;
TelemetrySender telemetrySender(Serial, INTERVAL); // Enviar cada 100ms
SwitchButton switchButton(SWITCH_PIN);
Hcsr04 hcsr04(TRIG_PIN, ECHO_PIN);
IrSensor irSensor(IR_PIN);
Battery battery(BATTERY_VOLTAGE_PIN);
LineSensor lineSensor(LINE_SENSOR_LEFT_PIN, LINE_SENSOR_MIDDLE_PIN, LINE_SENSOR_RIGHT_PIN);

void setup()
{
  Serial.begin(9600);

  // Inicializar los sensores
  switchButton.begin();
  hcsr04.begin();
  irSensor.begin();
  battery.begin();
  lineSensor.begin();

  // Esperar a que el serial este listo
  Serial.println("Atmega listo");
  delay(500);
}

void loop()
{
  // Actualizar timestamp
  telemetryFrame.t_ms = millis();

  // Actualizar sensores
  switchButton.update(telemetryFrame);
  // hcsr04.update(telemetryFrame);
  irSensor.update(telemetryFrame);
  battery.update(telemetryFrame);
  // lineSensor.update(telemetryFrame);

  // Enviar la telemetria
  telemetrySender.trySend(telemetryFrame);
}