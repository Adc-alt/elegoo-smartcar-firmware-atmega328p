#include <sensorServo.h>


SENSORSERVO::SENSORSERVO(uint8_t SERVO,uint8_t TRIG,uint8_t ECHO)
{
    this->pinSERVO=SERVO;
    this->pinTRIG=TRIG;
    this->pinECHO=ECHO;
};

