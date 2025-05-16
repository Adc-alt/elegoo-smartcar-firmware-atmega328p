#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <Arduino.h>

enum LINESENSOR_STATUS
{
    CENTERED,
    LEFT_DOMINANT,
    RIGHT_DOMINANT,
    LINE_LOST_RIGHT,
    LINE_LOST_LEFT,
};

class LINESENSOR
{
public:
    // Si ya sabemos cuales son los pines y que solo hay un line sensor
    //  en verdad esta bien que creemos la librería por si hubiera otro
    //  pero no tendríamos la obligación no o la necesidad?
    LINESENSOR(uint8_t LEFT, uint8_t MIDDLE, uint8_t RIGHT);
    LINESENSOR_STATUS getStatus();

    void updateStatus();
    void loop();
    void init();

private:
    LINESENSOR_STATUS status= CENTERED;

    uint8_t pinLEFT,pinMIDDLE,pinRIGHT;
};
#endif