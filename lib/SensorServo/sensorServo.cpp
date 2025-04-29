#include <sensorServo.h>

SENSORSERVO::SENSORSERVO(uint8_t SERVO, uint8_t TRIG, uint8_t ECHO)
{
    this->pinSERVO = SERVO;
    this->pinTRIG = TRIG;
    this->pinECHO = ECHO;

    pinMode(this->pinTRIG, OUTPUT);
    pinMode(this->pinECHO, INPUT);
};

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
    if (this->status == TURNING)
    {
        if ((millis() - startTurnintTime) >= this->servoDelay)
        {
            currentAngle = this->targetAngle;
            // currentAngle = map((millis() - startTurnintTime),0,servoDelay,currentAngle,targetAngle);
            this->status = IDLE;
            return;
        }
    }
}
void SENSORSERVO::updateOutputs()
{

    if (this->status == TURNING && this->previousStatus != TURNING)
    {
        servo.write(targetAngle);
        Serial.println((String) "Mandando angulo: " + targetAngle + this->pinSERVO);
        this->previousStatus = this->status;
        return;
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
    this->previousStatus = this->status;
}

void SENSORSERVO::getDistance()
{
    // this->status = GET_DISTANCE;
    // updateOutputs();
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
    // dsdsd

    this->servoDelay = ANGLE_TIME * abs(angle - this->currentAngle);
    this->targetAngle = angle;
    this->startTurnintTime = millis();
    this->status = TURNING;
}
