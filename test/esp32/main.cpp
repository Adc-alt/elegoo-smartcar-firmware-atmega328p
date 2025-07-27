// #include <Arduino.h>

// #define UART2_TX 40 // GPIO40 para TX
// #define UART2_RX 3  // GPIO3 para RX

// unsigned long receiveTime = 0; // Variable para guardar la hora de recepción
// unsigned long lastMessageTime = 0;
// const unsigned long TIMEOUT = 5000; // 5 segundos sin mensajes = desconectar

// void setup()
// {
//     Serial.begin(9600);
//     Serial2.begin(9600, SERIAL_8N1, UART2_RX, UART2_TX);

//     Serial.println("Esperando mensajes del ATmega...");
// }

// void loop()
// {
//     if (Serial2.available())
//     {
//         receiveTime = millis();
//         lastMessageTime = receiveTime; // Actualizar tiempo del último mensaje

//         String mensaje = Serial2.readStringUntil('\n');
//         mensaje.trim();

//         Serial.print("Recibido del ATmega: ");
//         Serial.print(mensaje);
//         Serial.print(" | Tiempo: ");
//         Serial.print(receiveTime);
//         Serial.println(" ms");
//     }

//     // Verificar timeout
//     if (millis() - receiveTime >900)
//     {
//         Serial.println("TIMEOUT - Desconectando micro...");
//         // Aquí puedes apagar el micro o hacer reset
//         esp_deep_sleep_start(); // Modo sleep profundo
//         // O: ESP.restart(); // Reiniciar
//         // O: esp_sleep_enable_timer_wakeup(0); // Apagar completamente
//     }
// }

#include <Arduino.h>

#define UART2_TX 40 // GPIO40 para TX
#define UART2_RX 3  // GPIO3 para RX

unsigned long receiveTime = 0; // Variable para guardar la hora de recepción
unsigned long lastMessageTime = 0;
const unsigned long TIMEOUT = 5000; // 5 segundos sin mensajes = desconectar

void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, UART2_RX, UART2_TX);

    Serial.println("Esperando mensajes del ATmega...");
}

void loop()
{
    if (Serial2.available())
    {
        receiveTime = millis();
        lastMessageTime = receiveTime;

        String mensaje = Serial2.readStringUntil('\n');
        mensaje.trim();

        // Serial.print("Recibido del ATmega: ");
        // Serial.print(mensaje);
        // Serial.print(" | Tiempo: ");
        // Serial.print(receiveTime);
        // Serial.println(" ms");

        // Responder con JSON
        Serial2.print("{\"response\":\"received\",\"data\":\"");
        Serial2.print(mensaje);
        Serial2.print("\",\"timestamp\":");
        Serial2.print(receiveTime);
        Serial2.println("}");
    }

    // // Verificar timeout
    // if (millis() - lastMessageTime > 900 && lastMessageTime > 0)
    // {
    //     Serial.println("TIMEOUT - Desconectando micro...");
    //     esp_deep_sleep_start();
    // }
}
