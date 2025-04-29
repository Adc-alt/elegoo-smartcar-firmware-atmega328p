#include <sensorServo.h>

SENSORSERVO::SENSORSERVO(uint8_t SERVO, uint8_t TRIG, uint8_t ECHO)
{
    this->pinSERVO = SERVO;
    this->pinTRIG = TRIG;
    this->pinECHO = ECHO;

    servo.attach(pinSERVO);
    pinMode(pinTRIG, OUTPUT);
    pinMode(pinECHO, INPUT);
};

SENSORSERVO_STATUS SENSORSERVO::getStatus()
{
    return this->status;
};

void SENSORSERVO::updateOutputs()
{
    if (this->status == IDLE)
    {
        digitalWrite(pinTRIG, LOW);
        return;
    }

    if (this->status == SCANNING)
    {
        digitalWrite(pinTRIG, LOW);
        timer.start();
        if (timer.hasElapsed(2))
        {
            timer.stop();
            digitalWrite(pinTRIG, HIGH);
        }

        timer.start();
        if (timer.hasElapsed(10))
        {
            timer.stop();
            digitalWrite(pinECHO, LOW);
            long duration = pulseIn(pinECHO, HIGH);
            return duration * 0.034 / 2;
        }
    }

    if (this->status == TURNING)
    {
        servo.write(this->targetAngle);
        if (targetAngle == currentAngle)
        {
            this->status = IDLE;
        }
        return;
    }
}

void SENSORSERVO::getDistance()
{
    this->status = GET_DISTANCE;
    updateOutputs();
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
//dsdsd
    this->targetAngle = angle; // Establecemos el ángulo objetivo
    this->status = TURNING;    // Cambiamos al estado de giro    
}
