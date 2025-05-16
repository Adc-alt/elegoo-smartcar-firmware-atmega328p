#include <lineSensor.h>
//
LINESENSOR::LINESENSOR(uint8_t LEFT, uint8_t MIDDLE, uint8_t RIGHT)
{
    this->pinLEFT = LEFT;
    this->pinMIDDLE = MIDDLE;
    this->pinRIGHT = RIGHT;
};

LINESENSOR_STATUS LINESENSOR::getStatus()
{
    return this->status;
};

void LINESENSOR::init() {
    //
};
