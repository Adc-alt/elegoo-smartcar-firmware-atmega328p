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

private:
  Stream& in;
  String buffer;
  bool processingMessage=false;
  unsigned long lastMessageTime=0;

  // Helper para parsear acción de motor
  MotorAction parseMotorAction(const char* action);
  LedColor parseLedColor(const char* color);
};