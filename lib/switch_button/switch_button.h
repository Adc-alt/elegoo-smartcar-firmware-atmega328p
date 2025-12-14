#pragma once

#include "../telemetry_frame/telemetry_frame.h"

#include <Arduino.h>
#include <elegoo_smart_car_lib.h>

class SwitchButton
{
public:
  // Constructor, le decimos que  pin usar en el main
  SwitchButton(uint8_t pin);

  // Se llama en el setup
  void begin();

  // Se llama en el loop
  void update(TelemetryFrame& frame);

private:
  uint8_t pin;
  bool isPressed      = false;
  bool lastPressed    = false;
  uint16_t pressCount = 0;
};
