// #include <IRremote.h>  // Comentado para evitar conflictos
#include "IrControl.h"

// 1. Constructor
IRCONTROL::IRCONTROL(uint8_t pinIR)
{
    this->pinIR = pinIR;
}

void IRCONTROL::inizializeIR()
{
    IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
}

// 2. Métodos públicos principales
void IRCONTROL::loop()
{
    updateOutput();
}

void IRCONTROL::updateOutput()
{
    // Implementar la lógica de actualización de salida
    if (manualMode == true)
    {
        this->status = IR_RECEIVING;
    }
    else
    {
        this->status = IR_IDLE;
    }
}

void IRCONTROL::decode()
{
    // Comentado temporalmente para evitar conflictos con IRremote en main.cpp
    // if (this->status == IR_RECEIVING)
    // {
    //     IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
    //     if (IrReceiver.decode())
    //     {
    //         switch (IrReceiver.decodedIRData.decodedRawData)
    //         {
    //         case 0xBC43FF00:
    //             this->statusCommand = IR_TURN_RIGHT;
    //             break;
    //         case 0xBB44FF00:
    //             this->statusCommand = IR_TURN_LEFT;
    //             break;
    //         case 0xB946FF00:
    //             this->statusCommand = IR_MOVE_FORWARD;
    //             break;
    //         case 0xEA15FF00:
    //             this->statusCommand = IR_MOVE_BACKWARD;
    //             break;
    //         default:
    //             this->statusCommand = IR_STOP;
    //             break;
    //         }
    //         IrReceiver.resume();
    //     }
    // }
}

IR_CONTROL_STATUS IRCONTROL::getStatus()
{
    return status;
}
IR_COMMAND_STATUS IRCONTROL::getStatusCommand()
{
    return statusCommand;
}

// 3. Métodos públicos auxiliares
String statusToString(IR_COMMAND_STATUS status)
{
    switch (status)
    {
    case IR_MOVE_FORWARD:
        return "MOVE_FORWARD";
    case IR_MOVE_BACKWARD:
        return "MOVE_BACKWARD";
    case IR_TURN_LEFT:
        return "TURN_LEFT";
    case IR_TURN_RIGHT:
        return "TURN_RIGHT";
    case IR_STOP:
        return "STOP";
    default:
        return "UNKNOWN";
    }
}