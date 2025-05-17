#include <HC-SR04.h>

HCSR04::HCSR04(uint8_t TRIG, uint8_t ECHO)
{
    this->pinTRIG = TRIG;
    this->pinECHO = ECHO;

    pinMode(this->pinTRIG, OUTPUT);
    pinMode(this->pinECHO, INPUT);
};
//
HCSR04_STATUS HCSR04::getStatus()
{
    return this->status;
};

void HCSR04::loop()
{
    updateOutputs();
}

void HCSR04::updateOutputs()
{
    if (this->status == HCS_SCANNING)
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
    this->distance = 0;
}

uint8_t HCSR04::getDistance()
{
    return this->distance;
}

void HCSR04::startScanning()
{
    this->status = HCS_SCANNING;
}
void HCSR04::stopScanning()
{
    this->status = HCS_IDLE;
}
