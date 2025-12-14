#include "battery.h"

// 1. Constructor
Battery::Battery(uint8_t pinVolt) : pinVolt(pinVolt)
{
}

void Battery::begin()
{
  pinMode(pinVolt, INPUT);
}

void Battery::update(TelemetryState& state)
{
  unsigned long now = millis();
  if (now - lastMeasureMs > measureTimeMs)
  {
    lastMeasureMs = now;
    // Leer voltaje
    this->voltage = readVoltage();
    // Actualizar estado
    updateStatus();
  }
  // Actualizar estado global, en la estructura global de telemetry state
  state.battery_voltage = this->voltage;
  state.battery_status  = this->status;
}

float Battery::readVoltage()
{
  // Podríamos agregar un filtro simple para suavizar la lectura, con un filtro de 5 medidas sería suficiente
  // progongo también aunque en este caso igual no es necesario un filtro de Kalman, solo para que os suene
  // no voy a implementar nada de esto porque añade complejidad innecesaria pero me gusta tenerlo en mente que lo
  // tengais en cuenta ya que si por ejemplo estais trabajando en el sector aeroespacial, por poner un ejemplo, todo se
  // mide al milimetros porque las aplicaciones son criticas así que no esta mál que lo investigueis
  int analogValue = analogRead(pinVolt); // Expplicar que arduino usa un ADC de 10 bits, por lo que el valor máximo es
                                         // 1023 y el voltaje de referencia es 5V
  // importante el voltaje maximo que se puede medir es el de referencia 5voltios por eso necesitamos el divisor de
  // tensión y por eso se multiplica luego
  float voltage = analogValue * 5.0 / 1024.0 * (11.5 / 1.5); // (R1+R2)/R2 = 11.5/1.5
  voltage += voltage * tolerance;                            // compensación por error estimado (8%)
  return voltage;
}

void Battery::updateStatus()
{
  if (this->voltage > voltageThreshold)
  {
    this->status = BatteryStatus::BatteryGood;
  }
  else
  {
    this->status = BatteryStatus::BatteryEmergency;
  }
}