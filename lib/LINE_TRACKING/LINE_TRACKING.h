#ifndef LINE_TRACKING_H
#define LINE_TRACKING_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

#define DETECT_THRESHOLD 400

enum LINE_TRACKING_STATUS
{
    CENTERED,
    CENTERED_SLIGHT_LEFT,
    CENTERED_SLIGHT_RIGHT,
    LEFT,
    RIGHT,
    LOST_PREV_LEFT,
    LOST_PREV_RIGHT,
    LOST
};

class LINE_TRACKING
{
public:
    LINE_TRACKING(uint8_t LEFT, uint8_t MIDDLE, uint8_t RIGHT); // Declaración del constructor, declaro los parametros que necesito meter para cuando instancie la clase
    LINE_TRACKING_STATUS getstatus();

    void forward(uint8_t vel);
    void reverse(uint8_t vel);
    void freeStop();
    void forceStop();

    void tick();

private:
    LINE_TRACKING_STATUS status = LOST;
    LINE_TRACKING_STATUS prevStatus = LOST; // Añadimos estado previo
    uint8_t pinLEFT, pinMIDDLE, pinRIGHT;

    uint16_t analogLeftValue, analogMiddleValue, analogRightValue;
    uint8_t digitalLeftValue, digitalMiddleValue, digitalRightValue;

    void convertAnalogToDigital();

    void updateOutputs();
};

#endif