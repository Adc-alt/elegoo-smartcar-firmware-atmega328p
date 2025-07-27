#include <Arduino.h>

unsigned long previousMillis = 0;
const unsigned long interval = 1000;
int i = 0;

void setup()
{
    Serial.begin(9600); // UART por hardware
    delay(1000);
    Serial.println("ATMEGA_READY"); // Señal de que está listo
}

void loop()
{
    // unsigned long currentMillis = millis();
    // if (currentMillis - previousMillis >= interval)
    // {
    //     previousMillis = currentMillis;
    //     Serial.println(i);
    //     i++;
    //     if (i >= 100)
    //         i = 0; // Reinicia en 100
    // }

    // Leer respuesta JSON del ESP32
    if (Serial.available())
    {
        String jsonResponse = Serial.readStringUntil('\n');
        jsonResponse.trim();
        Serial.print("JSON recibido del ESP32: ");
        Serial.println(jsonResponse);
    }
}
