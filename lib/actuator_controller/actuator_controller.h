#pragma once

#include "../motor/motor.h"

#include <ArduinoJson.h>
#include <FastLED.h>
#include <Servo.h>

/*
  ActuatorController
  ===================
  Controla los actuadores (LED, Servo y Motores) basándose en comandos JSON.

  Responsabilidades:
  - Procesar comandos de LED desde JSON
  - Procesar comandos de Servo desde JSON
  - Procesar comandos de Motores desde JSON
  - Mantener estado previo para optimizar escrituras

  No hace:
  - No recibe JSON (eso lo hace readJsonBySerial)
  - No parsea JSON
  - Solo ejecuta comandos en los actuadores
*/

// Enum para acciones de motor (permite usar switch-case)
enum class MotorAction
{
  FORWARD,
  REVERSE,
  TURN_LEFT,
  TURN_RIGHT,
  FORCE_STOP,
  FREE_STOP,
  UNKNOWN
};

class ActuatorController
{
public:
  // Constructor - recibe referencias a los actuadores
  ActuatorController(CRGB* leds, size_t numLeds, Servo& servo, MOTOR& leftMotor, MOTOR& rightMotor);

  // Procesar todos los comandos del JSON
  void processCommands(const JsonDocument& receiveJson);

private:
  // Referencias a los actuadores
  CRGB* leds;
  size_t numLeds;
  Servo& servo;
  MOTOR& leftMotor;
  MOTOR& rightMotor;

  // Estado previo para optimizar
  uint8_t previousServoAngle;
  char previousLedColor[10];

  // Métodos privados para procesar cada actuador
  void processLed(const char* color);
  void processServo(uint8_t angle);
  void processMotors(const char* action, uint8_t speed);

  // Helper: convertir string a enum (permite usar switch-case)
  MotorAction parseMotorAction(const char* action);
};
