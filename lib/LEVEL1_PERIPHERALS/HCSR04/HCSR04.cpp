#include "HCSR04.h"

// 1. Constructor
HCSR04::HCSR04(uint8_t TRIG, uint8_t ECHO)
{
    this->pinTRIG = TRIG;
    this->pinECHO = ECHO;

    pinMode(this->pinTRIG, OUTPUT);
    pinMode(this->pinECHO, INPUT);
}

// 2. Métodos públicos principales
HCSR04_STATUS HCSR04::getStatus()
{
    return this->status;
}

uint8_t HCSR04::getDistance()
{
    if ((millis() - startScanningTime) > 10)
    {
        digitalWrite(pinTRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(pinTRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(pinTRIG, LOW);

        long duration = pulseIn(pinECHO, HIGH, 15000); // 30 milisegundos, tiempo que tarda el sonido en recorrer la distancia y volver
        this->distance = duration * 0.0343 / 2;
        this->startScanningTime = millis();
        // Podría hacer más mediciones para obtener un valor más preciso, a modo de filtro de ruido
        // pero para reducir la complejidad del código, se hace una sola medición
    }
    // PRINT DISTANCE
    //  Serial.println((String) "HCSR04: distance: " + this->distance);
    return this->distance;
}

void HCSR04::loop()
{
    updateOutputs();
}

// 3. Control de operación
void HCSR04::startScanning()
{
    Serial.println((String) "HCSR04: startScanning");
    this->status = HCS_SCANNING;
}

void HCSR04::stopScanning()
{
    Serial.println((String) "HCSR04: stopScanning");
    this->status = HCS_IDLE;
}

// 4. Métodos privados de actualización
void HCSR04::updateOutputs()
{
    if (this->status == HCS_SCANNING)
    {
        this->distance = getDistance();
    }
}

// 5. Funciones auxiliares
String statusToString(HCSR04_STATUS status)
{
    switch (status)
    {
    case HCS_IDLE:
        return "IDLE";
    case HCS_SCANNING:
        return "SCANNING";
    default:
        return "UNKNOWN";
    }
}
