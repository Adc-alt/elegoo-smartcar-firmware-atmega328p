#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

// ===================== DEFINICIÓN DE ESTADOS =====================
enum SYSTEM_STATE
{
    STATE_IDLE = 0,
    STATE_ACTIVE = 1,
    STATE_MANUAL = 2,
};

// ===================== DEFINICIONES =====================

#define DEBOUNCE_DELAY 50

class STATE_MANAGER
{
public:
    STATE_MANAGER(uint8_t buttonPin = SWITCH_PIN);

    // Métodos principales
    void tick();
    void initialize();

    // Control de estados
    void setState(SYSTEM_STATE newState);
    void nextState();
    void goToPreviousState();
    SYSTEM_STATE getCurrentState();
    String stateToString(SYSTEM_STATE state);

    // Control del botón
    void enableButtonControl(bool enable);
    bool isButtonControlEnabled();

private:
    // Variables de estado
    SYSTEM_STATE currentState = STATE_IDLE;
    SYSTEM_STATE targetState = STATE_IDLE; // Para los cambios software
    SYSTEM_STATE previousState = STATE_IDLE;

    // Variables del botón
    uint8_t buttonPin;
    bool buttonPressed = false;         // Variable para almacenar el estado del botón
    bool lastButtonState = HIGH;        // Variable para almacenar el estado del botón en el último tick
    unsigned long lastDebounceTime = 0; // Variable para almacenar el tiempo de debounce
    bool buttonControlEnabled = true;   // Variable para controlar si el botón está habilitado
    bool softwareStateChange = false;   // Variable para controlar si el estado se ha cambiado por software

    // Métodos privados
    void readInputs();
    void updateStatus();
    void updateOutputs();
    void handleButtonPress();
};

#endif
