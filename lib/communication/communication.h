#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Communication
{
public:
  Communication();

  void begin(unsigned long baudRate = 9600);

  // Lee un mensaje completo y lo parsea en docReceive → devuelve true si llegó un JSON válido
  bool readMessage(JsonDocument& docReceive);

  // Envía un documento JSON con '\n'
  void sendMessage(const JsonDocument& docSend);

  // Comprueba timeout y escribe "TIMEOUT" si aplica
  void checkTimeout();

private:
  String buffer;                 // Acumula caracteres entrantes
  bool processingMessage;        // Si estamos leyendo un mensaje
  unsigned long lastMessageTime; // Última vez que recibimos un JSON válido
  unsigned long lastSentTime;    // Para controlar intervalo entre envíos

  static const unsigned long TIMEOUT_INTERVAL = 500;
  static const unsigned long INTERVAL         = 100;
};

#endif // COMMUNICATION_H
