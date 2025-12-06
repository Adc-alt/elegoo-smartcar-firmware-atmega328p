#include "SensorServo.h"
#include <math.h>

// 1. Constructor
SENSORSERVO::SENSORSERVO(HCSR04 &sensor, Servo &servo)
{
    this->sensor = &sensor;
    this->servo = &servo;
}

// 2. Métodos públicos principales
void SENSORSERVO::loop()
{
    this->sensor->loop(); // Esto en verdad es readInputs
    updateStatus();
    updateOutputs();
}

void SENSORSERVO::startScanning()
{
    if (this->status != IDLE)
        return;

    this->scanningState = SCAN_CENTER;
    this->nextSearchAngle = FRONT_ANGLE;
    this->searchIndex = 0;
    this->sensor->startScanning();
    this->status = SCANNING;
    Serial.println((String) "SensorServo: SCANNING");
}

void SENSORSERVO::startSearching()
{
    if (this->status != IDLE)
        return;

    this->objectAngle = NO_OBJECT_FOUND;
    this->nextSearchAngle = MIN_ANGLE;
    this->searchIndex = 0;
    this->sensor->startScanning();
    this->status = SEARCHING;
    Serial.println((String) "SensorServo: SEARCHING");
}

void SENSORSERVO::stop()
{
    this->status = IDLE;
    Serial.println((String) "SensorServo: IDLE");
}

// 3. Getters
SENSORSERVO_STATUS SENSORSERVO::getStatus()
{
    return this->status;
}

int SENSORSERVO::getSearchAngle()
{
    return this->objectAngle;
}

// 4. Setters
void SENSORSERVO::setAngle(uint8_t angle)
{
    Serial.println((String) "setAngle: " + angle);
    this->setAngle(angle, IDLE);
}

void SENSORSERVO::setAngle(uint8_t angle, SENSORSERVO_STATUS nextStatus)
{
    this->nextStatus = nextStatus;

    // Limites de angulo
    if (angle < MIN_ANGLE)
        angle = MIN_ANGLE;
    if (angle > MAX_ANGLE)
        angle = MAX_ANGLE;

    // Serial.println((String) "setAngle: " + angle + " desde: " + this->currentAngle);

    // Calcular tiempo usando regresión lineal
    this->servoDelay = calculateServoDelay(this->currentAngle, angle);
    this->targetAngle = angle;

    // Cambiar estado inmediatamente
    this->previousStatus = this->status;
    this->status = TURNING;
    this->startTurningTime = millis();
}

// 5. Métodos privados de actualización
void SENSORSERVO::updateStatus()
{
    if (this->status == TURNING)
    {
        if ((millis() - startTurningTime) >= this->servoDelay)
        {
            currentAngle = this->targetAngle;
            this->status = this->nextStatus;
        }
    }
}

void SENSORSERVO::updateOutputs()
{
    // 5.1 Manejo del estado TURNING - VERSIÓN MEJORADA
    if (this->status == TURNING)
    {
        // Enviar comando al servo continuamente mientras gira
        this->servo->write(targetAngle);
        return;
    }

    // 5.2 Manejo del estado SEARCHING
    if (this->status == SEARCHING)
    {
        if (this->sensor->getDistance() <= SEARCHING_THRESHOOLD)
        {
            // Serial.println((String) "SensorServo: OBJETO ENCONTRADO");
            this->objectAngle = this->currentAngle;
            this->setAngle(FRONT_ANGLE, IDLE);
        }

        if (this->objectAngle == NO_OBJECT_FOUND)
        {
            this->nextSearchAngle = MIN_ANGLE + searchIndex * SEARCHING_STEP;
            if (nextSearchAngle > MAX_ANGLE)
            {
                nextSearchAngle = MIN_ANGLE;
                searchIndex = 0;
            }
            this->setAngle(nextSearchAngle, SEARCHING);
            searchIndex++;
        }
    }

    // 5.3 Manejo del estado SCANNING
    if (this->status == SCANNING)
    {
        switch (this->scanningState)
        {
        case SCAN_CENTER:
            if (this->sensor->getDistance() <= SEARCHING_THRESHOOLD)
            {
                this->middleDistance = this->sensor->getDistance();
                this->currentAngle = FRONT_ANGLE;
                this->setAngle(MIN_ANGLE, SCANNING);
                this->scanningState = SCAN_LEFT;
                // Serial.println((String) "SensorServo SCANNING : SCAN CENTER");
            }
            break;
        case SCAN_LEFT:
            this->minDistance = this->sensor->getDistance();
            this->currentAngle = MIN_ANGLE;
            this->setAngle(MAX_ANGLE, SCANNING);
            this->scanningState = SCAN_RIGHT;
            // Serial.println((String) "SensorServo SCANNING: SCAN LEFT");
            break;
        case SCAN_RIGHT:
            this->maxDistance = this->sensor->getDistance();
            this->scanningState = SCAN_COMPLETE;
            this->currentAngle = MAX_ANGLE;
            this->setAngle(FRONT_ANGLE, SCANNING);
            // Serial.println((String) "SensorServo: SCAN RIGHT - Distancia: " + this->maxDistance + "cm");
            Serial.println((String) "SensorServo: Resumen - IZQ: " + this->minDistance +
                           " CENTRO: " + this->middleDistance +
                           " DER: " + this->maxDistance);
            break;
        case SCAN_COMPLETE:
            this->scanningState = SCAN_COMPLETE;
            // Serial.println((String) "SensorServo: ESCANEO COMPLETADO - IDLE");
            break;
        }
    }
}

// 6. Métodos de cálculo
unsigned long SENSORSERVO::calculateServoDelay(uint8_t currentAngle, uint8_t targetAngle)
{
    // Fórmula final (simple, con fundamento físico):
    // t_ms = 100 + 27 * sqrt(Δθ)
    int delta_theta = abs(targetAngle - currentAngle);

    if (delta_theta == 0)
    {
        return 0;
    }

    unsigned long delay_ms = 100 + 27 * sqrt(delta_theta);
    return delay_ms;
}

// 7. Funciones auxiliares
String statusToString(SENSORSERVO_STATUS status)
{
    switch (status)
    {
    case IDLE:
        return "IDLE";
    case TURNING:
        return "TURNING";
    case SCANNING:
        return "SCANNING";
    case SEARCHING:
        return "SEARCHING";
    default:
        return "UNKNOWN";
    }
}