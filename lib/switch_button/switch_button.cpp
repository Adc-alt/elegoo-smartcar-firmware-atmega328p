#include <switch_button.h>

SwitchButton::SwitchButton(uint8_t pin)

{
  this->pin = pin;
}

void SwitchButton::begin()
{
  pinMode(this->pin, INPUT_PULLUP);
}
void SwitchButton::update(TelemetryFrame& frame)
{
  // Leemos el boton, en bajo quiere decir que se ha pulsado(pullUp)
  isPressed = (digitalRead(this->pin) == LOW);

  if (isPressed && !lastPressed)
  {
    pressCount++; // Se aumenta contador
  }

  lastPressed = isPressed; // Actualizamos el estado anterior

  // Actualizar el frame global
  frame.sw_pressed = isPressed;
  frame.sw_count   = pressCount;
}
