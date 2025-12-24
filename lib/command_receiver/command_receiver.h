#pragma once
#include "../command_frame/command_frame.h"

#include <Arduino.h>
#include <ArduinoJson.h>

/*
  CommandReceiver
  ==============
  Recibe JSON del ESP32 por Serial y actualiza el CommandFrame.

  Responsabilidades:
  - Leer JSON del Stream (Serial)
  - Parsear el JSON
  - Actualizar CommandFrame con los comandos recibidos

  No hace:
  - No ejecuta los comandos (eso lo hacen los actuadores)
  - No interpreta los comandos
  - Solo recibe y actualiza el frame de comandos
*/
class CommandReceiver
{
public:
  explicit CommandReceiver(Stream& in);
  bool tryReceive(CommandFrame& commandFrame);
  void checkTimeout(unsigned long timeoutMs = 5000);
  bool isReceiving() const
  {
    return processingMessage;
  } // Para que TelemetrySender sepa si puede enviar

private:
  Stream& in;
  static const size_t BUFFER_SIZE = 256; // Buffer estático - no fragmenta memoria
  char buffer[BUFFER_SIZE];
  size_t bufferIndex            = 0;
  bool processingMessage        = false;
  unsigned long lastMessageTime = 0;
  // Helper para parsear acción de motor
  MotorAction parseMotorAction(const char* action);
  LedColor parseLedColor(const char* color);

  // Helpers para parseo manual de JSON (sin objetos anidados - ahorra RAM)
  const char* findJsonKey(const char* json, const char* key);
  int extractIntValue(const char* start);
  const char* extractStringValue(const char* start, char* output, size_t maxLen);
};