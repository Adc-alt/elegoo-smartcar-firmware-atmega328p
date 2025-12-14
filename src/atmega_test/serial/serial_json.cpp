#include "../lib/led_rgb/led_rgb.h"
#include "Arduino.h"

#include <ArduinoJson.h>
#include <IRremote.h>
#include <elegoo_smart_car_lib.h>

// LED_RGB ledRGB(4);

// Timeouts
#define TIMEOUT_INTERVAL 500
#define INTERVAL 100

// Estado de recepción
String buffer; // Buffer esto sirve para guardar el mensaje que vamos recibiendo

// Vamos a crear dos mensajes json, el que se envia y el que se recibe(Globales)
JsonDocument docSend;
JsonDocument docReceive;

bool processingMessage        = false; // Bandera que nos servira para identificar el timeout
bool button                   = false;
bool ledstate                 = false;
unsigned long lastMessageTime = 0;
unsigned long lastSentTime    = 0; // ← Variable global para timing

// Declaramos los prototipos de las funciones
void sendMessage();
void readMessage();
void checkTimeout();
void processMessageJSON();

// Declaramos la funcion setup
void setup()
{
  Serial.begin(9600);
  // ledRGB.inizializeLEDRGB();
  delay(1500);
  pinMode(SWITCH_PIN, INPUT);

  // Inicializar receptor IR para depuración
  Serial.println("Inicializando receptor IR en pin " + String(IR_PIN) + "...");
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  delay(100);

  Serial.println("=== ATMEGA LISTO ===");
  Serial.println("Esperando señales IR...");
  Serial.println("Formato: Protocolo | Direccion | Comando | RawData (hex)");
  Serial.println("----------------------------------------");
}

void loop()
{
  // Depuración del sensor IR
  if (IrReceiver.decode())
  {
    Serial.println("=== SEÑAL IR DETECTADA ===");

    // Información básica
    Serial.print("Protocolo: ");
    Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

    Serial.print("Dirección: 0x");
    Serial.println(IrReceiver.decodedIRData.address, HEX);

    Serial.print("Comando: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    // Raw data (el valor que usamos en el código)
    Serial.print("RawData (dec): ");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, DEC);

    Serial.print("RawData (hex): 0x");
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    // Información adicional
    Serial.print("Número de bits: ");
    Serial.println(IrReceiver.decodedIRData.numberOfBits);

    Serial.print("Flags: 0x");
    Serial.println(IrReceiver.decodedIRData.flags, HEX);

   
    Serial.println("----------------------------------------");

    // Reanudar recepción
    IrReceiver.resume();
  }
}
// void sendMessage()
//   {
//     unsigned long currentTime = millis();

//     if (currentTime - lastSentTime >= INTERVAL)
//     {
//       lastSentTime = currentTime;
//       serializeJson(docSend, Serial);
//       Serial.write('\n');
//     }
//   }

//   void readMessage()
//   {
//     while (Serial.available())
//     {
//       char c = Serial.read(); // Voy almacenando
//       if (!processingMessage)
//       {
//         processingMessage = true;
//       }

//       if (c == '\n')
//       {
//         if (buffer.length() > 0)
//         {
//           DeserializationError err = deserializeJson(docReceive, buffer);
//           if (err)
//           {
//             // Serial.print("JSON inválido: ");
//             Serial.println(err.f_str());
//           }
//           else
//           {
//             // Serial.println("JSON recibido y válido:");
//             lastMessageTime = millis(); // ← TIMEOUT empieza AQUÍ (después de procesar)
//             // serializeJsonPretty(docReceive, Serial);
//             // Serial.write('\n');
//             // Serial.println(docReceive.as<String>());

//             // Procesar el mensaje INMEDIATAMENTE después de recibirlo
//             processMessageJSON();
//           }
//         }
//         buffer            = "";
//         processingMessage = false; // ← Reset cuando termina el mensaje
//       }
//       else if (c != '\r')
//       {
//         buffer += c;
//       }
//     }
//   }

//   void checkTimeout()
//   { // Actualmente no tiene ninguna utilidad pero en el futuro
//     // puede servir para interrumpir procesos importantes si se
//     // pierde la comunicacion
//     // unsigned long currentTime = millis();
//     // EL tiempo se actualiza con el ultimo mensaje
//     if ((millis() - lastMessageTime) > TIMEOUT_INTERVAL)
//     {
//       processingMessage = false;
//       Serial.println("TIMEOUT");
//     }
//   }

//   void processMessageJSON()
//   {
//     const char* type = docReceive["type"];
//     if (!type)
//     {
//       // No hay "type", ignoramos el mensaje
//       return;
//     }

//     // --- PETICIÓN GET ---
//     if (strcmp(type, "get") == 0)
//     {
//       const char* target = docReceive["target"];
//       if (!target)
//         return;

//       // GET button1 → leer el botón y responder
//       if (strcmp(target, "button1") == 0)
//       {
//         bool buttonState = !digitalRead(SWITCH_PIN); // pull-up → activo en LOW

//         docSend.clear();
//         docSend["type"]   = "resp";
//         docSend["target"] = "button1";
//         docSend["value"]  = buttonState ? 1 : 0;

//         sendMessage(); // Envía {"type":"resp","target":"button1","value":0/1}
//       }
//     }

//     // --- PETICIÓN SET ---
//     if (strcmp(type, "cmd") == 0)
//     {
//       const char* target = docReceive["target"];
//       if (!target)
//         return;

//       // Comando para LED
//       if (strcmp(target, "led1") == 0)
//       {
//         int value = docReceive["value"]; // 0 o 1

//         if (value == 1)
//         {
//           ledRGB.setGreen();
//         }
//       }
//     }
//   }