#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

#define N_MEDIA 5

enum HCSR04_STATUS
{
    HCS_IDLE,
    HCS_SCANNING
};

class HCSR04
{
public:
    HCSR04(uint8_t TRIG, uint8_t ECHO);

    HCSR04_STATUS getStatus();
    uint8_t getDistance();

    void startScanning();
    void stopScanning();
    void loop();

private:
    void updateOutputs();

    HCSR04_STATUS status = HCS_IDLE;

    uint8_t pinTRIG, pinECHO;
    uint8_t distance;

    unsigned long startScanningTime = 0;

    // uint8_t distances[N_MEDIA] = {0};
};

#endif
