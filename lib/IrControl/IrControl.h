#ifndef IR_CONTROL_H
#define IR_CONTROL_H

#include <Arduino.h>
#include <IRremote.h>

// 1. Enums y constantes públicas
enum IRCONTROL_STATUS
{
    IR_IDLE,         // Cambiado de IDLE
    IR_RECEIVING,    // Cambiado de RECEIVING
    IR_MOVEFORWARD,  // Cambiado de MOVEFORWARD
    IR_MOVEBACKWARD, // Cambiado de MOVEBACKWARD
    IR_TURNLEFT,     // Cambiado de TURNLEFT
    IR_TURNRIGHT,    // Cambiado de TURNRIGHT
    IR_STOP,
};

class IRCONTROL
{
public:
    // Constructor
    IRCONTROL(uint8_t pinIR);

    // Métodos públicos principales
    void setManualMode(bool active);
    void decode();
    void loop();
    void begin();

    // Getters
    IRCONTROL_STATUS getStatus();

private:
    // Variables de estado
    IRCONTROL_STATUS status = IR_IDLE;

    // Variables de configuración
    uint8_t pinIR;
    bool manualMode = false;
};

// 12. Funciones auxiliares
String statusToString(IRCONTROL_STATUS status);

#endif