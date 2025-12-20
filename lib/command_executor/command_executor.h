#pragma once

#include "../command_frame/command_frame.h"
#include "../led_rgb/led_rgb.h"
#include "../motor/motor.h"

#include <Servo.h>

/*
  CommandExecutor
  ===============
  Ejecuta los comandos recibidos del ESP32 en los actuadores físicos.

  Responsabilidades:
  - Leer del CommandFrame
  - Ejecutar comandos en motores, servo y LED
  - Resetear flags después de ejecutar

  No hace:
  - No recibe comandos (eso lo hace CommandReceiver)
  - No parsea JSON
  - Solo ejecuta lo que está en el CommandFrame
*/
class CommandExecutor
{
public:
  // Constructor - recibe referencias a los actuadores
  CommandExecutor(MOTOR& leftMotor, MOTOR& rightMotor, Servo& servo, LED_RGB& led);

  // Ejecutar todos los comandos pendientes en el frame
  void execute(const CommandFrame& commandFrame);

private:
  // Referencias a los actuadores
  MOTOR& leftMotor;
  MOTOR& rightMotor;
  Servo& servo;
  LED_RGB& led;

  // Métodos privados para ejecutar cada tipo de comando
  void executeMotors(const CommandFrame& commandFrame);
  void executeServo(const CommandFrame& commandFrame);
  void executeLed(const CommandFrame& commandFrame);
};
