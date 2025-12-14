#include <Arduino.h>
#include <ArduinoJson.h>
#include <battery.h>
#include <elegoo_smart_car_lib.h>
#include <hcsr04.h>
#include <ir_sensor.h>
#include <switch_button.h>
#include <telemetry_sender.h>
#include <telemetry_state.h>

// Definiciones
#define INTERVAL 100

// Objetos
TelemetryState telemetryState;
TelemetrySender telemetrySender(Serial, INTERVAL); // Enviar cada 100ms
SwitchButton switchButton(SWITCH_PIN);
Hcsr04 hcsr04(TRIG_PIN, ECHO_PIN);
IrSensor irSensor(IR_PIN);
Battery battery(BATTERY_VOLTAGE_PIN);

void setup()
{
  Serial.begin(9600);

  // Inicializar los sensores
  switchButton.begin();
  hcsr04.begin();
  irSensor.begin();
  battery.begin();
}

void loop()
{
  // Actualizar el timestamp
  telemetryState.t_ms = millis();

  // Actualizar el estado del boton
  switchButton.update(telemetryState);
  hcsr04.update(telemetryState);
  irSensor.update(telemetryState);
  battery.update(telemetryState);
  
  // Enviar la telemetria
  telemetrySender.trySend(telemetryState);
}