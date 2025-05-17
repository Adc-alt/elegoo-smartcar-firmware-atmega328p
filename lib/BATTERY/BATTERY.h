#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

enum BATTERY_STATUS
{
    GOOD,
    EMERGENCY
};


class BATTERY
{
public:
    BATTERY(uint8_t VOLT); // Declaración del constructor, declaro los parametros que necesito meter para cuando instancie la clase
    BATTERY_STATUS getstatus();

    void getVoltage();    
    void tick();

private:
    BATTERY_STATUS status = GOOD;    
    uint8_t pinVOLT;
    // bool in1,in2 = false;
    unsigned long lastMeasureTime = 0;
    void updateOutputs();
};

#endif