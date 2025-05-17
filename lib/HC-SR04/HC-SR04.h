#ifndef HCSR04_H
#define HCSR04_H

#include <ElegooSmartCar.h>
#include <Arduino.h>


#define NUM_MEASUREMENTS 4
#define INTERVAL 50


enum HCSR04_STATUS
{
    IDLE_HC,
    SCANNING_HC
};

class HCSR04
{
public:
    HCSR04(uint8_t TRIG, uint8_t ECHO);
    HCSR04_STATUS getStatus();

    void startScanning();
    void stopScanning();
    void loop();

    uint8_t getDistance();
    
private:    
    void updateStatus();
    HCSR04_STATUS status = IDLE_HC;
    
    uint8_t pinTRIG, pinECHO;
    uint8_t distance;
    unsigned long startScanningTime = 0;
    unsigned long startTime = 0;
    bool started = false;
    bool measured = false;

    const unsigned long interval = 50; // tiempo entre mediciones (ms)
    const int numMeasures = 4;
    float measures[NUM_MEASUREMENTS] = {0}; // Creamos un array
    int index = 0;

    void updateOutputs();
};

#endif