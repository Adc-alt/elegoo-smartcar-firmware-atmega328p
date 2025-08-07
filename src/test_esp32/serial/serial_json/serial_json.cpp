#include <Arduino.h>
#include <ArduinoJson.h>


void setup()
{    

    Serial.begin(9600);
    Serial.println("Serial JSON Test - Enviando JSON...");

}


void loop()
{
    JsonDocument doc;
    doc["Sensor"]=26.1;
    doc["Humedad"]=74;
    doc["Calor"]="mucho";
    
    //Serializamos
    serializeJson(doc,Serial);
    // Serial.println();
    
}

