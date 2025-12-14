#ifndef APP_H
#define APP_H

#include "led_rgb.h" // tu clase del LED

#include <Arduino.h>
#include <ArduinoJson.h>


class App
{
public:
  App(LED_RGB& led);

  void begin();

  // Procesa un JSON recibido y escribe la respuesta en docSend.
  // Si no hay respuesta que enviar, docSend quedará vacío.
  void handleMessage(const JsonDocument& docReceive, JsonDocument& docSend);

private:
  LED_RGB& ledRGB;
};

#endif
