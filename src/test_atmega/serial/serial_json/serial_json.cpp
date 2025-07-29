#include <Arduino.h>
#include <ArduinoJson.h>

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial JSON Test - Esperando JSON...");

    // // Crear JSON y serializarlo
    // JsonDocument doc;
    // doc["sensor"] = "gps";
    // doc["time"] = 1351824120;

    // Crear array
    // JsonArray data = doc.createNestedArray("data");
    // data.add(1);
    // data.add(2);
    // data.add(3);

    // Serializar a string
    // char output[256];
    // serializeJson(doc, output);
    // Serial.println("JSON serializado:");
    // Serial.println(output);
}

void printJsonValue(JsonVariant value)
{
    if (value.is<const char *>())
    {
        Serial.println(value.as<const char *>());
        return;
    }

    if (value.is<int>())
    {
        Serial.println(value.as<int>());
        return;
    }

    if (value.is<long>())
    {
        Serial.println(value.as<long>());
        return;
    }

    if (value.is<float>())
    {
        Serial.println(value.as<float>());
        return;
    }

    if (value.is<JsonObject>())
    {
        Serial.println("[Objeto]");
        JsonObject nested = value.as<JsonObject>();
        for (JsonPair nestedKv : nested)
        {
            Serial.print("  ");
            Serial.print(nestedKv.key().c_str());
            Serial.print(": ");
            Serial.println(nestedKv.value().as<float>());
        }
        return;
    }

    Serial.println("[Tipo desconocido]");
}

void readJson()
{
    // Esperar JSON desde ESP32 por Serial
    if (Serial.available())
    {
        String receivedJson = Serial.readStringUntil('\n');
        receivedJson.trim(); // Quitar espacios y saltos de línea

        Serial.println("JSON recibido desde ESP32:");
        Serial.println(receivedJson);

        // Deserializar el JSON recibido
        JsonDocument doc; //Buffer de 1024 bytes
        DeserializationError error = deserializeJson(doc, receivedJson);

        if (error)
        {
            Serial.print("Error al deserializar: ");
            Serial.println(error.c_str());
        }
        else
        {
            Serial.println("JSON procesado correctamente:");

            // Recorrer todos los campos del JSON dinámicamente
            for (JsonPair kv : doc.as<JsonObject>())
            {
                Serial.print("Campo: ");
                Serial.print(kv.key().c_str());
                Serial.print(" = ");

                // Mostrar el valor según su tipo
                printJsonValue(kv.value());
            }
        }
        Serial.println("---");
    }
}

void loop()
{

    readJson();
}
