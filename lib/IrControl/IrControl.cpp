#include <IrControl.h>

// 1. Constructor
IRCONTROL::IRCONTROL(uint8_t pinIR)
{   
    this->pinIR = pinIR;
    // status = IR_IDLE;
}

void IRCONTROL::begin()
{
    IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
}

// 2. Métodos públicos principales
void IRCONTROL::loop()
{
    decode();
}

void IRCONTROL::decode()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.decodedRawData == 0xBC43FF00)
        {
            this->status = IR_TURNRIGHT;
        }
        if (IrReceiver.decodedIRData.decodedRawData == 0xBB44FF00)
        {
            this->status = IR_TURNLEFT;
        }
        if (IrReceiver.decodedIRData.decodedRawData == 0xB946FF00)
        {
            this->status = IR_MOVEFORWARD;
        }
        if (IrReceiver.decodedIRData.decodedRawData == 0xEA15FF00)
        {
            this->status = IR_MOVEBACKWARD;
        }
        else
        {
            this->status = IR_IDLE;
        }
        IrReceiver.resume();
    }
}

IRCONTROL_STATUS IRCONTROL::getStatus()
{
    return status;
}

// 3. Métodos públicos auxiliares
String statusToString(IRCONTROL_STATUS status)
{
    switch (status)
    {
    case IR_IDLE:
        return "IDLE";
    case IR_RECEIVING:
        return "RECEIVING";
    case IR_MOVEFORWARD:
        return "MOVEFORWARD";
    case IR_MOVEBACKWARD:
        return "MOVEBACKWARD";
    case IR_TURNLEFT:
        return "TURNLEFT";
    case IR_TURNRIGHT:
        return "TURNRIGHT";
    case IR_STOP:
        return "STOP";
    default:
        return "UNKNOWN";
    }
}