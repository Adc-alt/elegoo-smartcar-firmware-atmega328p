#include <battery.h>

// Definicion del constructor
BATTERY::BATTERY(uint8_t PINVOLT)
{
    this->pinVOLT = PINVOLT; // Asingnación atemporal del pin
    pinMode(PINVOLT, INPUT);
}

BATTERY_STATUS BATTERY::getstatus()
{
    return this->status;
}

void BATTERY::tick()
{
    updateOutputs();
    if (millis() - lastMeasureTime > 1000)
    {
        lastMeasureTime = millis();

        int analogValue = analogRead(VOLTAGE_PIN);
        float voltage = analogValue * 5.0 / 1024.0 * (11.5 / 1.5); // (R1+R2)/R2 = 11.5/1.5
        voltage += voltage * 0.08;                                 // compensación por error estimado (8%)

        // Serial.print("Voltaje actual: ");
        // Serial.print(voltage);
        // Serial.println(" V");

        if (voltage > 7.8)
        {
            this->status=GOOD;            
        }
        else
        {
            this->status=EMERGENCY;            
        }
    }
}

void BATTERY::updateOutputs()
// Lo que hacemos aquí es actualizar las salidas dependiendo del estado
// en el que estemos
{
    if (this->status == GOOD)
    {
        Serial.println("Batería cargada completamente");
        return;
    }
    if (this->status == EMERGENCY)
    {
        Serial.println("CARGA LA BATERÏA HAY UN PROBLEMA");
        return;
    }
}

