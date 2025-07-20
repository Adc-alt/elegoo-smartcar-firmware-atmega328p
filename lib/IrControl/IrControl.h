#ifndef IR_CONTROL_H
#define IR_CONTROL_H

#include <Arduino.h>
// Forward declaration para evitar múltiples definiciones
class IRrecv;

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
    void setManualMode(bool active);
    void loop();

    // Getters
    IR_CONTROL_STATUS getStatus();
    IR_COMMAND_STATUS getStatusCommand();

    // Variables públicas
    bool manualMode = false;

private:
    // Variables de estado
    IR_CONTROL_STATUS status = IR_IDLE;
    IR_COMMAND_STATUS statusCommand = IR_STOP;

    // Métodos privados
    void inizializeIR();
    void decode();
    void updateOutput();

    // Variables de configuración
    uint8_t pinIR;

    // Variables para auto-reset
    unsigned long lastIRTime = 0;
    const unsigned long IR_TIMEOUT = 600; // 200ms timeout
};

// 12. Funciones auxiliares
String statusToString(IR_CONTROL_STATUS status);
String statusToString(IR_COMMAND_STATUS status);

#endif