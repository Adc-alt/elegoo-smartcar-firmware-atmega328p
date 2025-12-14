#include <switch_button.h>

SwitchButton::SwitchButton(uint8_t pin)

{
  this->pin = pin;
}

void SwitchButton::begin()
{
  pinMode(this->pin, INPUT_PULLUP);
}
void SwitchButton::update(TelemetryState& state)  
{
  // Leemos el boton, en bajo quiere decir que se ha pulsado(pullUp)
  isPressed = (digitalRead(this->pin) == LOW);

  if (isPressed && !lastPressed)
  {
    pressCount++; // Se aumenta contador
    updateStatus();
  }

  lastPressed = isPressed; // Actualizamos el estado anterior

  // Actualizar el estado global
  state.sw_pressed = isPressed;
  state.sw_count   = pressCount;
  state.sw_mode    = status;
}

void SwitchButton::updateStatus()
{
  switch ((pressCount - 1) % 4)
  {
    case 0:
      status = SwitchButtonStatus::Mode1;
      break;
    case 1:
      status = SwitchButtonStatus::Mode2;
      break;
    case 2:
      status = SwitchButtonStatus::Mode3;
      break;
    case 3:
      status = SwitchButtonStatus::Mode4;
      break;
  }
}
SwitchButtonStatus SwitchButton::getStatus()
{
  return status;
}
