#include <sensorServo.h>

SENSORSERVO::SENSORSERVO(HCSR04 &sensor, Servo &servo) {

};
//
SENSORSERVO_STATUS SENSORSERVO::getStatus()
{
    return this->status;
};

void SENSORSERVO::loop()
{
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
            return;
        }
    }
}
void SENSORSERVO::updateOutputs()
{
    if (this->status == TURNING && this->previousStatus != TURNING)
    {
        this->servo->write(targetAngle);
        Serial.println((String) "Mandando angulo: " + targetAngle);
        this->previousStatus = this->status;
        return;
    }

    if (this->status == SEARCHING)
    {
        // OBJETO ENCONTRADO
        if (this->sensor->getDistance() <= SEARCHING_THRESHOOLD)
        {
            this->objectAngle = this->currentAngle;
        }

        if (this->objectAngle != -1)
        {
            this->setAngle(FRONT_ANGLE, IDLE);
        }
        if (this->objectAngle == -1)
        {
            if (nextSearchAngle > MAX_ANGLE)
            {
                nextSearchAngle = MIN_ANGLE;
                searchIndex = 1;
            }
            this->setAngle(nextSearchAngle, SEARCHING);
            this->nextSearchAngle = MIN_ANGLE + searchIndex * SEARCHING_STEP;
            searchIndex++;
        }
    }

    this->previousStatus = this->status;
}

void SENSORSERVO::startScanning()
{
    if (this->status != IDLE)
    {
        return;
    }
    this->status = SCANNING;
}
void SENSORSERVO::startSearching()
{
    if (this->status != IDLE)
    {
        return;
    }
    this->objectAngle = -1;
    this->nextSearchAngle = MIN_ANGLE;
    this->searchIndex = 1;
    this->status = SEARCHING;
}
void SENSORSERVO::stop()
{
    this->status = IDLE;
}

void SENSORSERVO::setAngle(uint8_t angle)
{
    this->setAngle(angle, IDLE);
}
void SENSORSERVO::setAngle(uint8_t angle, SENSORSERVO_STATUS nextStatus)
{

    if (this->status != IDLE)
    {
        return;
    }
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
}

int SENSORSERVO::getSearchAngle()
{
    return this->objectAngle;
}