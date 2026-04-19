#include "battery.h"

// 1. Constructor
Battery::Battery(uint8_t pinVolt) : pinVolt(pinVolt)
{
}

void Battery::begin()
{
  pinMode(pinVolt, INPUT);
}

float Battery::getVoltage()
{
  unsigned long now = millis();
  if (now - lastMeasureMs > measureTimeMs)
  {
    lastMeasureMs = now;
    // Leer voltaje
    this->voltage = readVoltage();
  }
  return voltage;
}

float Battery::readVoltage()
{
  // ADC 10 bits, Vref ~5 V. El divisor (R1+R2)/R2 = 11.5/1.5 acota el pin al rango del ADC.
  // Si hace falta más estabilidad, se puede añadir una media móvil (no implementada aquí).
  int analogValue = analogRead(pinVolt);
  float voltage = analogValue * 5.0 / 1024.0 * (11.5 / 1.5); // (R1+R2)/R2 = 11.5/1.5
  voltage += voltage * tolerance;                            // compensación por error estimado (8%)
  return voltage;
}