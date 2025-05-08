#include <sensorServo.h>

SENSORSERVO::SENSORSERVO(uint8_t SERVO, uint8_t TRIG, uint8_t ECHO)
{
    this->pinSERVO = SERVO;
    this->pinTRIG = TRIG;
    this->pinECHO = ECHO;

    pinMode(this->pinTRIG, OUTPUT);
    pinMode(this->pinECHO, INPUT);
};
//
SENSORSERVO_STATUS SENSORSERVO::getStatus()
{
    return this->status;
};

void SENSORSERVO::init()
{
    this->servo.attach(this->pinSERVO);
}
void SENSORSERVO::loop()
{
    updateStatus();
    updateOutputs();
}

void SENSORSERVO::updateStatus()
{
    if (this->status == SCANNING)
    {
        if (!this->isScanning)
        {
            this->status = IDLE;
            return;
        }
    }
    if (this->status == TURNING)
    {
        if ((millis() - startTurningTime) >= this->servoDelay)
        {
            currentAngle = this->targetAngle;
            this->status = IDLE;
            return;
        }
    }
    else if (this->status == IDLE)
    {
        // Automáticamente iniciamos una medición cuando estamos en IDLE
        this->status = SCANNING;
        return;
    }
    else if (this->status == SCANNING)
    {
        return;
    }
}
void SENSORSERVO::updateOutputs()
{
    if (this->status == TURNING && this->previousStatus != TURNING)
    {
        servo.write(targetAngle);
        Serial.println((String) "Mandando angulo: " + targetAngle);
        this->previousStatus = this->status;
        return;
    }
    if (this->status == SCANNING)
    {
        if ((millis() - startScanningTime) <= 1)
        {
            digitalWrite(pinTRIG, HIGH);
        }
        if ((millis() - startScanningTime) >= 2 && (millis() - startScanningTime) < 12)
        {
            digitalWrite(pinTRIG, LOW);
        }
        if ((millis() - startScanningTime) >= 12)
        {
            digitalWrite(pinECHO, LOW);
            long duration = pulseIn(pinECHO, HIGH);
            this->distance = duration * 0.034 / 2;
            this->startScanningTime = millis();
        }
    }

    if (this->status == SCANNING)
    {
        unsigned long currentMillis = millis();

        if (!started)
        {
            startTime = currentMillis;
            started = true;
            measured = false;
        }

        unsigned long elapsed = currentMillis - startTime;

        if (elapsed < 2)
        {
            digitalWrite(this->pinTRIG, LOW);
        }
        else if (elapsed < 12)
        {
            digitalWrite(this->pinTRIG, HIGH);
        }
        else if (!measured)
        {
            digitalWrite(this->pinTRIG, LOW);
            long duration = pulseIn(this->pinECHO, HIGH, 25000);
            float distancia = duration * 0.034 / 2.0;

            // Guardar medida en el array circular
            measures[index] = distance;
            index = (index + 1) % numMeasures;

            // Calcular promedio
            float suma = 0;
            for (int i = 0; i < numMeasures; i++)
            {
                suma += measures[i];
            }
            this->distance = suma / numMeasures;

            Serial.print("Distancia promedio: ");
            Serial.println(this->distance);

            measured = true;
            this->status = IDLE;
        }
        else if (elapsed > interval)
        {
            started = false;
        }
    }

    this->previousStatus = this->status;
}

uint8_t SENSORSERVO::getDistance()
{
    this->status = SCANNING;
    return this->distance;
}

void SENSORSERVO::startScanning()
{
    this->isScanning = true;
    this->startScanningTime = millis();
}
void SENSORSERVO::stopScanning()
{
    this->isScanning = false;
}
void SENSORSERVO::setAngle(uint8_t angle)
{
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

// if (this->status == IDLE)
// {
//     digitalWrite(pinTRIG, LOW);
//     return;
// }

// if (this->status == SCANNING)
// {
//     digitalWrite(pinTRIG, LOW);
//     timer.start();
//     if (timer.hasElapsed(2))
//     {
//         timer.stop();
//         digitalWrite(pinTRIG, HIGH);
//     }

//     timer.start();
//     if (timer.hasElapsed(10))
//     {
//         timer.stop();
//         digitalWrite(pinECHO, LOW);
//         long duration = pulseIn(pinECHO, HIGH);
//         // return duration * 0.034 / 2;
//         return;
//     }
// }

// if (this->status == TURNING)
// {
//     servo.write(this->targetAngle);
//     if (targetAngle == currentAngle)
//     {
//         this->status = IDLE;
//     }
//     return;
// }