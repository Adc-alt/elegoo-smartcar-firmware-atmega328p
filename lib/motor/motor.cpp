#include "motor.h"
// #include <hardwarePins.h>

// Definición del constructor
MOTOR::MOTOR(uint8_t IN1, uint8_t PWM, uint8_t stby)
{
  this->pinIN1  = IN1; // Asingnación atemporal del pin
  this->pinPWM  = PWM; // Asingnación atemporal del pin PWM
  this->pinSTBY = stby;
  pinMode(pinIN1, OUTPUT);
  pinMode(pinPWM, OUTPUT);
}

MOTOR_STATUS MOTOR::getStatus()
{
  return this->status;
}

void MOTOR::tick()
{
  // readInputs();
  // updateStatus();
  // this->updateOutputs();
}

void MOTOR::updateOutputs()
{
  if (this->status == FREE_STOP)
  {
    digitalWrite(pinSTBY, LOW);
  }
  if (this->status == FORCE_STOP)
  {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinSTBY, HIGH);
    analogWrite(pinPWM, this->currentPwm);
    return;
  }
  if (this->status == FORWARD)
  {
    digitalWrite(pinIN1, HIGH);
    digitalWrite(pinSTBY, HIGH);
    analogWrite(pinPWM, this->currentPwm);
    return;
  }
  if (this->status == BACKWARD)
  {
    digitalWrite(pinIN1, LOW);
    digitalWrite(pinIN2, LOW);
    digitalWrite(pinSTBY, HIGH);
    analogWrite(pinPWM, this->currentPwm);
  }
}

void MOTOR::forward(uint8_t vel)
{
  this->currentPwm = map(vel, 0, 100, MIN_PWM, MAX_PWM);
  this->status     = FORWARD;
  this->updateOutputs();
}
void MOTOR::backward(uint8_t vel)
{
  this->currentPwm = map(vel, 0, 100, MIN_PWM, MAX_PWM);
  this->status     = BACKWARD;
  this->updateOutputs();
}
void MOTOR::freeStop()
{
  // this->currentPwm = 0;
  this->status = FREE_STOP;
  this->updateOutputs();
}
void MOTOR::forceStop()
{
  this->currentPwm = 0;
  this->status     = FORCE_STOP;
  this->updateOutputs();
}