#include "IR_CONTROL.h"
#include <IRremote.h>

IRCONTROL::IRCONTROL(uint8_t IR)
{
    pinIR = IR;
}

void IRCONTROL::setManualMode(bool active)
{
    manualMode = active;
    if (manualMode)
    {
        currentState = LISTENING;
        IrReceiver.begin(pinIR);
    }
    else
    {
        currentState = IDLE;
    }
}

void IRCONTROL::tick()
{
    if (!manualMode)
    {
        currentState = IDLE;
        return;
    }
    if (currentState == LISTENING)
    {
        if (IrReceiver.decode())
        {
            if (IrReceiver.decodedIRData.decodedRawData == 0xBC43FF00)
            {
                currentState = MOVEFORWARD;
            }
            IrReceiver.resume();
        }
    }
}

IRCONTROL::State IRCONTROL::getState() const
{
    return currentState;
}

