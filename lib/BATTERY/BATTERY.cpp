#include "Battery.h"

// 1. Constructor
BATTERY::BATTERY(uint8_t pinVolt)
{
    this->pinVolt = pinVolt;
    pinMode(pinVolt, INPUT);
}

// 2. Métodos públicos principales
BATTERY_STATUS BATTERY::getstatus()
{
    return this->status;
}

void BATTERY::loop()
{
    if (millis() - lastMeasureTime > MEASURE_TIME)
    {
        updateOutputs();
        lastMeasureTime = millis();
        this->voltage = getVoltage();
        if (voltage > 7.8)
        {
            this->status = GOOD;
        }
        else
        {
            this->status = EMERGENCY;
        }
    }
}

float BATTERY::getVoltage()
{
    int analogValue = analogRead(VOLTAGE_PIN);
    float voltage = analogValue * 5.0 / 1024.0 * (11.5 / 1.5); // (R1+R2)/R2 = 11.5/1.5
    voltage += voltage * 0.08;                                 // compensación por error estimado (8%)
    Serial.print("Voltaje actual: ");
    Serial.print(voltage);
    return voltage;
}

// 3. Métodos privados de actualización
void BATTERY::updateOutputs()
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
