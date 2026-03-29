#include "ir_sensor.h"

#include <Arduino.h> // Para Serial
#include <IRremote.h>
#include <stdint.h> // Asegurar que uint32_t esté definido

IrSensor::IrSensor(uint8_t pinIR) : pinIR(pinIR)
{
}

void IrSensor::begin()
{
  IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
}

uint32_t IrSensor::getIrRaw()
{
  if (IrReceiver.decode())
  {
    // Si es repetición, devolver 0 (no enviar comando repetido)
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
    {
      IrReceiver.resume();
      return 0; // No enviar repeticiones
    }

    uint32_t raw = IrReceiver.decodedIRData.decodedRawData;

    // Print del raw solo cuando hay comando nuevo
    // Serial.print(F("[IR] Raw: "));
    // Serial.println(raw);

    // Si por lo que sea llega 0, ignóralo
    if (raw != 0)
    {
      irCommand = raw;
      IrReceiver.resume();
      return raw; // Devolver el comando solo cuando es nuevo
    }

    IrReceiver.resume();
  }

  // Si no hay nuevo comando, devolver 0
  return 0;
}