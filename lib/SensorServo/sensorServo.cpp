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
    if (this->status == SCANNING)
    {
        // this->status = IDLE;
        return;
    }

    if (this->status == IDLE)
    {
        return;
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

    if (this->status == SCANNING)
    {
        unsigned long currentMillis = millis();

        if (!started)
        {
            this->setAngle(FRONT_ANGLE, IDLE);
        }
        if (this->objectAngle == -1)
        {
            if (nextSearchAngle > MAX_ANGLE)
            {
                nextSearchAngle = MIN_ANGLE;
                searchIndex = 1;
            digitalWrite(this->pinTRIG, LOW);
            long duration = pulseIn(this->pinECHO, HIGH, 25000);
            float distancia = duration * 0.034 / 2.0;

            // Guardar medida en el array circular
            measures[index] = distancia;
            index = (index + 1) % numMeasures;

            // Calcular promedio
            float suma = 0;
            for (int i = 0; i < numMeasures; i++)
            {
                suma += measures[i];
            }
            this->setAngle(nextSearchAngle, SEARCHING);
            this->nextSearchAngle = MIN_ANGLE + searchIndex * SEARCHING_STEP;
            searchIndex++;
            this->distance = suma / numMeasures;

            Serial.print("Distancia promedio: ");
            Serial.println(this->distance);

            measured = true;
            this->status = IDLE;
        }
        else if (elapsed > interval)
        {
            started = false; // reiniciar el ciclo de medición
        }
    }

    this->previousStatus = this->status;
    // this->status = IDLE;
}

uint8_t SENSORSERVO::getDistance()
{
    this->status = SCANNING;
    return this->distance;
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