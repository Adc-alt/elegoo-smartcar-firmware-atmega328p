#include "app.h"
#include "communication.h"
#include "led_rgb.h"

#include <Arduino.h>
#include <ArduinoJson.h>

LED_RGB ledRGB(4);
Communication comm;
App app(ledRGB);

// JSON globales
JsonDocument docReceive;
JsonDocument docSend;

void setup()
{
  comm.begin(9600);
  app.begin();

  Serial.println("ATmega listo");
}

void loop()
{
  // Si recibe un JSON válido
  if (comm.readMessage(docReceive))
  {
    // Procesa la lógica del mensaje
    app.handleMessage(docReceive, docSend);

    // Si la app creó una respuesta (docSend tiene datos), se envía
    if (!docSend.isNull())
    {
      comm.sendMessage(docSend);
      docSend.clear();
    }
  }

  // Tiempo sin mensajes
  comm.checkTimeout();
}
