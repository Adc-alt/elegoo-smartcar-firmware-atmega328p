#ifndef IR_CONTROL_H
#define IR_CONTROL_H

#include <Arduino.h>
#include <IRremote.h>

// 1. Enums y constantes públicas
enum IR_CONTROL_STATUS
{
    IR_IDLE,      // Cambiado de IDLE
    IR_RECEIVING, // Cambiado de RECEIVING
};

enum IR_COMMAND_STATUS
{
    IR_MOVE_FORWARD,
    IR_MOVE_BACKWARD,
    IR_TURN_LEFT,
    IR_TURN_RIGHT,
    IR_STOP,
};

class IRCONTROL
{
public:
    // Constructor
    IRCONTROL(uint8_t pinIR);

    // Métodos públicos principales
    // void setManualMode(bool active);
    void loop();
    void inizializeIR();

    // Getters
    IR_CONTROL_STATUS getStatus();
    IR_COMMAND_STATUS getStatusCommand();

private:
    // Variables de estado
    IR_CONTROL_STATUS status = IR_IDLE;
    IR_COMMAND_STATUS statusCommand = IR_STOP;

    // Métodos privados
    void decode();
    void updateOutput();

    // Variables de configuración
    uint8_t pinIR;
    bool manualMode = false;
};

// 12. Funciones auxiliares
String statusToString(IR_CONTROL_STATUS status);
String statusToString(IR_COMMAND_STATUS status);

#endif