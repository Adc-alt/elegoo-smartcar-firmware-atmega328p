#include "IrControl.h"
#include <IRremote.h>

// 1. Constructor
IRCONTROL::IRCONTROL(uint8_t pinIR)
{
    this->pinIR = pinIR;
}

// 2. Métodos públicos principales
// setManualMode: Activa o desactiva el modo manual actua como updateOutput
void IRCONTROL::setManualMode(bool active)
{
    this->manualMode = active;
    if (active == true)
    {
        this->status = IR_RECEIVING;
    }
    else
    {
        this->status = IR_IDLE;
    }
}

void IRCONTROL::inizializeIR()
{
    // Inicialización básica del pin
    pinMode(pinIR, INPUT);

    // Inicializar el receptor IR
    IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
    // Esperar a que el receptor IR se inicialice
    delay(50);
}

// 2. Métodos públicos principales
void IRCONTROL::loop()
{
    if (this->manualMode == true)
    {
        decode();
    }
}

void IRCONTROL::decode()
{
    if (this->status == IR_RECEIVING)
    {
        // NO inicializar en cada iteración - solo una vez al principio
        if (IrReceiver.decode())
        {
            lastIRTime = millis(); // Actualizar tiempo de última señal

            switch (IrReceiver.decodedIRData.decodedRawData)
            {
            case 0xF609FF00:
                this->statusCommand = IR_TURN_RIGHT;
                Serial.println("TURN_RIGHT");
                break;
            case 0xF807FF00:
                this->statusCommand = IR_TURN_LEFT;
                Serial.println("TURN_LEFT");
                break;
            case 0xBF40FF00:
                this->statusCommand = IR_MOVE_FORWARD;
                Serial.println("MOVE_FORWARD");
                break;
            case 0xE619FF00:
                this->statusCommand = IR_MOVE_BACKWARD;
                Serial.println("MOVE_BACKWARD");
                break;
            case 0xF30CFF00:
                this->statusCommand = IR_SERVO_LEFT;
                Serial.println("SERVO_LEFT");
                break;
            case 0xE718FF00:
                this->statusCommand = IR_SERVO_CENTER;
                Serial.println("SERVO_CENTER");
                break;
            case 0xA15EFF00:
                this->statusCommand = IR_SERVO_RIGHT;
                Serial.println("SERVO_RIGHT");
                break;
            case 0xFD00FF00:
            default:
                Serial.print("Código no reconocido: 0x");
                Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
                // NO cambiar el comando si no reconocemos la señal
                break;
            }
            IrReceiver.resume();
        }
        else
        {
            // Auto-reset después de timeout
            if (millis() - lastIRTime > IR_TIMEOUT && lastIRTime != 0)
            {
                this->statusCommand = IR_STOP;
            }
        }
    }
}

IR_CONTROL_STATUS IRCONTROL::getStatus()
{
    // Serial.println("IR_CONTROL_STATUS: " + statusToString(status));
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

String statusToString(IR_CONTROL_STATUS status)
{
    switch (status)
    {
    case IR_IDLE:
        return "IDLE";
    case IR_RECEIVING:
        return "RECEIVING";
    default:
        return "UNKNOWN";
    }
}