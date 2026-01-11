#include "ir_sensor.h"

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
    // Si es repetición, no machaques el último comando válido
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)
    {
      IrReceiver.resume();
      return irCommand; // mantén el último bueno
    }

    uint32_t raw = IrReceiver.decodedIRData.decodedRawData;

    // Si por lo que sea llega 0, ignóralo
    if (raw != 0)
    {
      irCommand = raw;
    }

    IrReceiver.resume();
  }
  return irCommand;
}