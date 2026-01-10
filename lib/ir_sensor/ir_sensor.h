#pragma once

#include <Arduino.h>
#include <stdint.h>

class IrSensor
{
public:
  explicit IrSensor(uint8_t pinIR);
  void begin();

  // Devuelve el valor raw hexadecimal directamente (0xBC43FF00, 0xBB44FF00, etc.)
  // Retorna 0 si no hay señal
  uint32_t getIrRaw();

  // Obtener el último comando IR detectado
  uint32_t getIrCommand() const
  {
    return irCommand;
  }

private:
  uint8_t pinIR;
  uint32_t irCommand = 0; // Último comando IR detectado
};