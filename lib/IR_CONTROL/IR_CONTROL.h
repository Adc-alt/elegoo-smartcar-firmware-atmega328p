#ifndef IRCONTROL_H
#define IRCONTROL_H

#include <Arduino.h>

class IRCONTROL
{
public:
    enum State
    {
        IDLE,
        LISTENING,
        MOVEFORWARD
    };

    IRCONTROL(uint8_t IR);
    void setManualMode(bool active);
    void tick();
    State getState() const;

private:
    uint8_t pinIR;
    bool manualMode = false;
    State currentState = IDLE;
};

#endif // IRCONTROL_H