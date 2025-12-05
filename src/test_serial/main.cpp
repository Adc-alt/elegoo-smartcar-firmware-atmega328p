#include <Arduino.h>
#include <ArduinoJson.h>
#include "../lib/elegoo_smartcar_lib.h"


// Timeouts
#define TIMEOUT_INTERVAL 500
#define INTERVAL 100

// Estado de recepción
String buffer;

//Vamos a crear dos mensajes json, el que se envia y el que se recibe(Globales)
JsonDocument docSend;
JsonDocument docReceive;
JsonDocument docRequest;


bool   processingMessage     = false; // Bandera que nos servira para identificar el timeout 
bool button=false;
unsigned long lastMessageTime   = 0;
unsigned long lastSentTime      = 0;  // ← Variable global para timing


//Declaramos los prototipos de las funciones 
void sendMessage(JsonDocument& doc);
void readMessage();
void checkTimeout();
void processMessageJSON();

//Declaramos la funcion setup
void setup() 
{
  Serial.begin(9600);                                
  Serial2.begin(9600, SERIAL_8N1, UART2_RX, UART2_TX); 
  delay(1500);

  docRequest.clear();
  docRequest["type"]="order";
  docRequest["cmd"]="getButton";
  Serial.println("ESP32 listo para recibir JSON por UART");
}



void loop() 
{
  // Enviar petición periódica "getButton" #1
  sendMessage(docRequest);

  readMessage();

  checkTimeout();

}
  


void sendMessage(JsonDocument& doc)
{
  unsigned long currentTime = millis();

  if (currentTime - lastSentTime >= INTERVAL)
  {
    lastSentTime = currentTime;
    serializeJson(doc, Serial2);  // Usa el parámetro en lugar de docSend
    Serial2.write('\n');    
  }
}

// Sobrecarga: envía docSend sin parámetros
void sendMessage()
{
  unsigned long currentTime = millis();

  if (currentTime - lastSentTime >= INTERVAL)
  {
    lastSentTime = currentTime;
    serializeJson(docSend, Serial2);
    Serial2.write('\n');    
  }
}

void readMessage() 
{
  while (Serial2.available()) 
  {        
    char c = Serial2.read(); // Voy almacenando        
    if (!processingMessage) 
    {
      processingMessage = true;
    }

    if (c == '\n') 
    {      
      DeserializationError err = deserializeJson(docReceive, buffer);
      if (err) 
      {
        // Serial.print("JSON inválido: ");
        // Serial.println(err.f_str());
      } else 
      { 
        // Serial.println("JSON recibido y válido:");
        lastMessageTime = millis();  // ← TIMEOUT empieza AQUÍ (después de procesar)
        serializeJsonPretty(docReceive, Serial);
        Serial.write('\n');

        processMessageJSON();
      }
      buffer = "";   
      processingMessage = false;  // ← Reset cuando termina el mensaje
    } else if (c != '\r') 
      {
      buffer += c; 
        }
  }
}

void checkTimeout()// Actualmente no tiene ninguna utilidad pero en el futuro puede servir para interrumpir procesos importantes si se pierde la comunicacion
{ 
  // EL tiempo se actualiza con el ultimo mensaje
  if ((millis() - lastMessageTime) > TIMEOUT_INTERVAL)
  {
    processingMessage = false;
    Serial.println("TIMEOUT");
  }
}


void processMessageJSON()
{
  // 1. Verificar si viene el campo "cmd" para conocer el contexto
  if (!docReceive["cmd"].isNull()) 
  {
    const char* cmd = docReceive["cmd"];
    
    // Procesar según el tipo de comando
    if (strcmp(cmd, "getButton") == 0) 
    {
      // Respuesta a getButton - verificar si viene con el dato del botón
      if (!docReceive["button"].isNull()) 
      {
        bool buttonState = docReceive["button"].as<bool>();
        
        if (buttonState) 
        {
          docSend.clear();
          docSend["cmd"] = "powerLed";
          docSend["state"] = true;
          
          serializeJson(docSend, Serial2);
          Serial2.write('\n');
        }
      }
    }
  }
  // Limpiar el json recibido
  docReceive.clear();
}