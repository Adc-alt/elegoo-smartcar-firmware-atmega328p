#pragma once
#include <stdint.h>

/*
  CommandFrame
  ============
  Representa los comandos recibidos del ESP32 para controlar los actuadores.

  Este es el equivalente en el ATmega de lo que SensorData es en el ESP32.

  - Se actualiza cuando llega un comando del ESP32
  - Es el punto de verdad único para los comandos en el ATmega
  - Los actuadores leen de aquí y ejecutan

  Principios:
  - No hay lógica
  - No hay hardware
  - No hay JSON
  - Solo variables que representan los comandos actuales
*/

// Enum para acciones de motor
enum class MotorAction
{
  NONE,       // Sin comando (mantener estado actual)
  FORWARD,    // Avanzar
  REVERSE,    // Retroceder
  FORCE_STOP, // Parada forzada
  FREE_STOP   // Parada libre
};

// Enum para colores del LED
enum class LedColor
{
  NONE, // Sin comando (mantener estado actual)
  BLACK,
  BLUE,
  RED,
  YELLOW,
  PURPLE,
  GREEN
};

struct CommandFrame
{
  // ---- Metadatos ----
  uint32_t lastUpdate_ms = 0;     // Última vez que se recibió un comando
  bool hasNewCommand     = false; // Flag para saber si hay comando nuevo

  // ---- Motores ----
  struct
  {
    MotorAction leftAction  = MotorAction::NONE;
    uint8_t leftSpeed       = 0; // 0-100
    MotorAction rightAction = MotorAction::NONE;
    uint8_t rightSpeed      = 0; // 0-100
  } motors;

  // ---- Servo ----
  bool servoHasCommand = false;
  uint8_t servoAngle   = 90; // 0-180 grados

  // ---- LED ----
  bool ledHasCommand = false;
  LedColor ledColor  = LedColor::NONE;
};