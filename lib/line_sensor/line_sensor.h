#pragma once
#include "../telemetry_frame/telemetry_frame.h"

#include <Arduino.h>

class LineSensor
{
public:
  // 3. Constructor
  LineSensor(uint8_t pinLeft, uint8_t pinMiddle, uint8_t pinRight);

  // 4. Métodos públicos principales
  void begin();
  void update(TelemetryFrame& frame);

private:
  // 6. Variables de configuración
  uint8_t pinLeft;
  uint8_t pinMiddle;
  uint8_t pinRight;

  // 5. Constantes de configuración
  //   static constexpr uint8_t detectThreshold = 400;

  // 7. Variables de lectura
  uint16_t analogLeftValue   = 0;
  uint16_t analogMiddleValue = 0;
  uint16_t analogRightValue  = 0;
};
