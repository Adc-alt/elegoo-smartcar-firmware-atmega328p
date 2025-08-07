#include "StateManager.h"

// ===================== CONSTRUCTOR =====================
STATE_MANAGER::STATE_MANAGER(uint8_t buttonPin)
{
    this->buttonPin = buttonPin;
}

// ===================== MÉTODOS PRINCIPALES =====================

void STATE_MANAGER::tick()
{
    readInputs();
    updateStatus();
    updateOutputs();
}

void STATE_MANAGER::readInputs()
{
    if (!buttonControlEnabled)
        return;

    bool reading = digitalRead(buttonPin);

    // Detectar cambio de estado
    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    // Anti-rebote
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        if (reading == LOW && !buttonPressed)
        {
            buttonPressed = true;
            handleButtonPress();
        }
        else if (reading == HIGH)
        {
            buttonPressed = false;
        }
    }

    lastButtonState = reading;
}

void STATE_MANAGER::updateStatus()
{
    // Control por software del estado
    if (softwareStateChange)
    {
        previousState = currentState;
        currentState = targetState;
        softwareStateChange = false;

        Serial.print("Cambio de estado por software - Estado actual: ");
        Serial.println(stateToString(currentState));
    }
}

void STATE_MANAGER::updateOutputs()
{
    // Aquí puedes agregar lógica específica para cada estado
    // Por ejemplo, controlar LEDs, motores, etc.
    switch (currentState)
    {
    case STATE_IDLE:
        // Estado IDLE - Reposo
        // Serial.println("Estado: IDLE - Sistema en reposo");
        break;

    case STATE_ACTIVE:
        // Estado ACTIVE - Funcionalidad activa
        // Serial.println("Estado: ACTIVE - Sistema activo");
        break;

        // case STATE_PROCESSING:
        // Estado PROCESSING - Procesando
        // Serial.println("Estado: PROCESSING - Procesando datos");
        // break;

        // case STATE_EMERGENCY:
        // Estado EMERGENCY - Emergencia
        // Serial.println("Estado: EMERGENCY - Modo emergencia");
        // break
    }
}

// ===================== INICIALIZACIÓN =====================

void STATE_MANAGER::initialize()
{
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.println("StateManager inicializado");
    Serial.print("Estado inicial: ");
    Serial.println(stateToString(currentState));
}

void STATE_MANAGER::setState(SYSTEM_STATE newState)
{
    targetState = newState;
    softwareStateChange = true;
}

// ===================== CONTROL DE ESTADOS =====================

void STATE_MANAGER::nextState()
{
    // Aqui se realizan varias cosas,
    // 1. Se convierte enum a numero
    // 2. Se hace el calculo, la suma para el siguiente estado

    // Esto es el static_cast conversión de tipos en C++
    // es como decir hey se lo que hago convierte el desitno al origenstatic_cast<DESTINO>(ORIGEN)

    int nextStateIndex = (static_cast<int>(currentState) + 1) % 4;
    // 3. Se convierte el numero a enum
    targetState = static_cast<SYSTEM_STATE>(nextStateIndex);
    softwareStateChange = true;
}

void STATE_MANAGER::goToPreviousState()
{
    int prevStateIndex = (static_cast<int>(currentState) - 1 + 4) % 4;
    targetState = static_cast<SYSTEM_STATE>(prevStateIndex);
    softwareStateChange = true;
}

SYSTEM_STATE STATE_MANAGER::getCurrentState()
{
    return currentState;
}

String STATE_MANAGER::stateToString(SYSTEM_STATE state)
{
    switch (state)
    {
    case STATE_IDLE:
        return "IDLE";
    case STATE_ACTIVE:
        return "ACTIVE";
    case STATE_MANUAL:
        return "MANUAL";
    // case STATE_PROCESSING:
    //     return "PROCESSING";
    // case STATE_EMERGENCY:
    //     return "EMERGENCY";
    default:
        return "UNKNOWN";
    }
}

// ===================== MÉTODOS PRIVADOS =====================

void STATE_MANAGER::handleButtonPress()
{
    if (!buttonControlEnabled)
        return;

    int nextStateIndex = (static_cast<int>(currentState) + 1) % 4;
    currentState = static_cast<SYSTEM_STATE>(nextStateIndex);

    Serial.print("Botón presionado - Estado actual: ");
    Serial.println(stateToString(currentState));
}

// ESTE CONTROL AÑADIRÍA SEGURIDAD PERO NO QUIERO AÑADIR COMPLEJIDAD
// ===================== CONTROL DEL BOTÓN =====================

// void STATE_MANAGER::enableButtonControl(bool enable)
// {
//     buttonControlEnabled = enable;
// }

// bool STATE_MANAGER::isButtonControlEnabled()
// {
//     return buttonControlEnabled;
// }
