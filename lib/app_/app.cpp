#include "app.h"

App::App(LED_RGB& led) : ledRGB(led)
{
}

void App::begin()
{
  // Inicializa hardware de la app
  pinMode(SWITCH_PIN, INPUT);
  ledRGB.inizializeLEDRGB();
}

void App::handleMessage(const JsonDocument& in, JsonDocument& out)
{
  const char* type = in["type"];
  if (!type)
    return;

  const char* target = in["target"];
  if (!target)
    return;

  // ---- handle peticiones GET  ---- button1
  if (strcmp(type, "get") == 0 && strcmp(target, "button1") == 0)
  {
    bool buttonState = !digitalRead(SWITCH_PIN);

    out.clear();
    out["type"]   = "resp";
    out["target"] = "button1";
    out["value"]  = buttonState ? 1 : 0;

    return;
  }

  // ---- Handle peticiones CMD  ---- led1
  if (strcmp(type, "cmd") == 0 && strcmp(target, "led1") == 0)
  {
    int value = in["value"];

    if (value == 1)
      ledRGB.setGreen();
    // En comandos normalmente no respondes (o sí, tú decides)
    return;
  }
}
