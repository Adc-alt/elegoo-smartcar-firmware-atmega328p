#include <sensorServo.h>

SENSORSERVO::SENSORSERVO(HCSR04 &sensor, Servo &servo)
{
    this->sensor = &sensor;
    this->servo = &servo;
}
//
SENSORSERVO_STATUS SENSORSERVO::getStatus()
{
    return this->status;
};

void SENSORSERVO::loop()
{
    this->sensor->loop();
    updateStatus();
    updateOutputs();
}

void SENSORSERVO::updateStatus()
{

    if (this->status == TURNING)
    {
        if ((millis() - startTurningTime) >= this->servoDelay)
        {
            currentAngle = this->targetAngle;
            this->status = this->nextStatus;
            Serial.println((String) "SensorServo: TURNING->" + statusToString(this->nextStatus));
            return;
        }
    }
}
void SENSORSERVO::updateOutputs()
{
    if (this->status == TURNING && this->previousStatus != TURNING)
    {
        this->servo->write(targetAngle);
        Serial.println((String) "servo->write: " + targetAngle);
        this->previousStatus = this->status;
        return;
    }

    if (this->status == SEARCHING)
    {
        // OBJETO ENCONTRADO
        if (this->sensor->getDistance() <= SEARCHING_THRESHOOLD)
        {
            Serial.println((String) "SensorServo: OBJETO ENCONTRADO");
            this->objectAngle = this->currentAngle;
            this->setAngle(FRONT_ANGLE, IDLE);
        }

        if (this->objectAngle == -1)
        {
            this->nextSearchAngle = MIN_ANGLE + searchIndex * SEARCHING_STEP;
            Serial.println((String) "SensorServo: nextSearchAngle: " + nextSearchAngle);
            if (nextSearchAngle > MAX_ANGLE)
            {
                nextSearchAngle = MIN_ANGLE;
                searchIndex = 0;
            }
            this->setAngle(nextSearchAngle, SEARCHING);
            searchIndex++;
        }
    }
}

void SENSORSERVO::startScanning()
{
    if (this->status != IDLE)
    {
        return;
    }
    this->status = SCANNING;
    Serial.println((String) "SensorServo: SCANNING");
}
void SENSORSERVO::startSearching()
{
    if (this->status != IDLE)
    {
        return;
    }
    this->objectAngle = -1;
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

void SENSORSERVO::setAngle(uint8_t angle)
{
    Serial.println((String) "setAngle: " + angle);
    this->setAngle(angle, IDLE);
}
void SENSORSERVO::setAngle(uint8_t angle, SENSORSERVO_STATUS nextStatus)
{

    // if (this->status != IDLE)
    // {
    //     Serial.println((String) "setAngle: ERROR, status != IDLE");
    //     return;
    // }
    this->nextStatus = nextStatus;
    if (angle < MIN_ANGLE)
    {
        angle = MIN_ANGLE;
    }
    if (angle > MAX_ANGLE)
    {
        angle = MAX_ANGLE;
    }

    this->servoDelay = ANGLE_TIME * abs(angle - this->currentAngle);
    this->targetAngle = angle;
    this->startTurningTime = millis();
    this->status = TURNING;
    Serial.println((String) "SensorServo: TURNING -> " + angle + " delay: " + this->servoDelay);
}

int SENSORSERVO::getSearchAngle()
{
    return this->objectAngle;
}

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