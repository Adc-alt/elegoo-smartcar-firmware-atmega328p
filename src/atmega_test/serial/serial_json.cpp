#include "Arduino.h"
#include <ArduinoJson.h>
#include <elegoo_smart_car_lib.h>
#include "../lib/led_rgb/led_rgb.h"  


LED_RGB ledRGB(4);

// Timeouts
#define TIMEOUT_INTERVAL 500
#define INTERVAL 100

// Estado de recepción
String buffer; // Buffer esto sirve para guardar el mensaje que vamos recibiendo

//Vamos a crear dos mensajes json, el que se envia y el que se recibe(Globales)
JsonDocument docSend;
JsonDocument docReceive;


bool   processingMessage     = false; // Bandera que nos servira para identificar el timeout 
bool button=false;
bool ledstate=false;
unsigned long lastMessageTime   = 0;
unsigned long lastSentTime      = 0;  // ← Variable global para timing


//Declaramos los prototipos de las funciones
void sendMessage();
void readMessage();
void checkTimeout();
void processMessageJSON();

//Declaramos la funcion setup
void setup() 
{
  Serial.begin(9600);  
  ledRGB.inizializeLEDRGB();
  delay(1500);
  pinMode(SWITCH_PIN,INPUT);

  Serial.println("Atmega listo");
}

void loop() 
{
  readMessage();  
  
  if (ledstate) ledRGB.setGreen();
  
  checkTimeout();
}


void sendMessage()
{
  unsigned long currentTime = millis();
  
  if (currentTime - lastSentTime >= INTERVAL)
  {
    lastSentTime = currentTime;
    serializeJson(docSend,  Serial);
    Serial.write('\n');
  }
}

void readMessage() 
{
  while (Serial.available()) 
  {        
    char c = Serial.read(); // Voy almacenando        
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
        Serial.println(err.f_str());
      } else 
      { 
        // Serial.println("JSON recibido y válido:");
        lastMessageTime = millis();  // ← TIMEOUT empieza AQUÍ (después de procesar)
        serializeJsonPretty(docReceive, Serial);        
        Serial.write('\n');
        Serial.println(docReceive.as<String>());
        
        // Procesar el mensaje INMEDIATAMENTE después de recibirlo
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
  // unsigned long currentTime = millis();
  // EL tiempo se actualiza con el ultimo mensaje
  if ((millis() - lastMessageTime) > TIMEOUT_INTERVAL)
  {
    processingMessage = false;
    Serial.println("TIMEOUT");
  }
}

void processMessageJSON()
{
   if (!docReceive["cmd"].isNull()) 
   {
     const char* cmd = docReceive["cmd"];
     
     // Comando getButton
     if (strcmp(cmd, "getButton") == 0 )  
     {
       // Leer el estado del botón desde el sensor
       bool buttonState = !digitalRead(SWITCH_PIN); // ! porque el botón está en pull-up
   
       // Preparar y enviar la respuesta con el estado del botón
       docSend["cmd"] = "getButton";
       docSend["button"] = buttonState;
       sendMessage();
       docSend.clear();
      //  Serial.println("Caracola");
     }
     
     // Comando powerLed
     if (strcmp(cmd, "powerLed") == 0) 
     {
       bool state = docReceive["state"];
       if (state == true) 
       {
         ledstate = true;
       }
     }
   }
}



