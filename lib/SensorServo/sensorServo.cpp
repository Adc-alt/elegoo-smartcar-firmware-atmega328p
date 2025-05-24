#include "SensorServo.h"

// 1. Constructor
SENSORSERVO::SENSORSERVO(HCSR04 &sensor, Servo &servo)
{
    this->sensor = &sensor;
    this->servo = &servo;
}

// 2. Métodos públicos principales
void SENSORSERVO::loop()
{
    this->sensor->loop();
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

    this->servoDelay = calculateServoDelay(this->currentAngle, angle);
    this->targetAngle = angle;
    this->startTurningTime = millis();

    Serial.println((String) "SensorServo: currentState: " + statusToString(this->status));
    this->previousStatus = this->status;
    this->status = TURNING;
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
    // 5.1 Manejo del estado TURNING
    if (this->status == TURNING && this->previousStatus != TURNING)
    {
        Serial.println((String) "SensorServo: Escribiendo ángulo " + targetAngle +
                       " (previousStatus: " + statusToString(this->previousStatus) + ")");
        this->servo->write(targetAngle);
        this->previousStatus = this->status;
        return;
    }

    // 5.2 Manejo del estado SEARCHING
    if (this->status == SEARCHING)
    {
        if (this->sensor->getDistance() <= SEARCHING_THRESHOOLD)
        {
            Serial.println((String) "SensorServo: OBJETO ENCONTRADO");
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
                this->setAngle(MIN_ANGLE, SCANNING);
                this->scanningState = SCAN_LEFT;
            }
            break;
        case SCAN_LEFT:
            this->minDistance = this->sensor->getDistance();
            this->setAngle(MAX_ANGLE, SCANNING);
            this->scanningState = SCAN_RIGHT;
            break;
        case SCAN_RIGHT:
            this->maxDistance = this->sensor->getDistance();
            Serial.println((String) "SensorServo: Resumen - IZQ: " + this->minDistance +
                           " CENTRO: " + this->middleDistance +
                           " DER: " + this->maxDistance);
            this->scanningState = SCAN_COMPLETE;
            this->setAngle(FRONT_ANGLE, IDLE);
            break;
        case SCAN_COMPLETE:
            // No hacer nada o lo que quieras
            break;
        }
    }
}

// 6. Métodos de cálculo
unsigned long SENSORSERVO::calculateServoDelay(uint8_t currentAngle, uint8_t targetAngle)
{
    return 60.55 * sqrt(abs(targetAngle - currentAngle)) + 49.92;
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