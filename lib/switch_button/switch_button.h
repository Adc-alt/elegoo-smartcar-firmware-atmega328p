#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include "../telemetry_state/telemetry_state.h"

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
  void update(TelemetryState& state);

  // Devuelve el modo actual
  SwitchButtonStatus getStatus();

private:
  // Pin físico dle boton
  uint8_t pin;

  // Está pulsado ahora mismo?
  bool isPressed = false;

  // Estaba pulsado en la iteración anterior
  bool lastPressed = false;

  // Cuantas veces se presiona el botón
  uint16_t pressCount = 0;

  // Modo en el que comienza
  SwitchButtonStatus status = SwitchButtonStatus::Mode1;

  // Funcion interna para setear el estado según el contador
  void updateStatus();
};

#endif
