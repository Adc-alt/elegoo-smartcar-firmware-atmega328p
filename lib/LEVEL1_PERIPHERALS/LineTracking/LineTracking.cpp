#include "LineTracking.h"

// 1. Constructor
LINE_TRACKING::LINE_TRACKING(uint8_t pinLeft, uint8_t pinMiddle, uint8_t pinRight)
{
    this->pinLeft = pinLeft;
    this->pinMiddle = pinMiddle;
    this->pinRight = pinRight;

    pinMode(pinLeft, INPUT);
    pinMode(pinMiddle, INPUT);
    pinMode(pinRight, INPUT);
}

// 2. Métodos públicos principales
LINE_TRACKING_STATUS LINE_TRACKING::getstatus()
{
    return this->status;
}

void LINE_TRACKING::loop()
{
    // Guardamos el estado actual antes de actualizarlo
    this->prevStatus = this->status;

    // Actualizamos las lecturas
    readInputs();
    updateStatus();
}

// 3. Métodos privados de actualización
void LINE_TRACKING::readInputs()
{
    this->digitalLeftValue = (analogRead(this->pinLeft) > DETECT_THRESHOLD);
    this->digitalMiddleValue = (analogRead(this->pinMiddle) > DETECT_THRESHOLD);
    this->digitalRightValue = (analogRead(this->pinRight) > DETECT_THRESHOLD);
}

void LINE_TRACKING::updateStatus()
{
    // Lógica de estados

    // Estado LOST: Anormal
    if (this->digitalLeftValue == false && this->digitalMiddleValue == false && this->digitalRightValue == false)
    {
        // Si ya estamos en un estado LOST_PREV, lo mantenemos
        if (this->status == LOST_PREV_LEFT || this->status == LOST_PREV_RIGHT)
        {
            return;
        }

        // Si perdimos la línea por primera vez, usamos el estado previo para determinar la dirección
        if (this->prevStatus == RIGHT || this->prevStatus == CENTERED_SLIGHT_RIGHT)
        {
            this->status = LOST_PREV_RIGHT;
        }
        else if (this->prevStatus == LEFT || this->prevStatus == CENTERED_SLIGHT_LEFT)
        {
            this->status = LOST_PREV_LEFT;
        }
        else
        {
            this->status = LOST;
        }
    }
    // Si no estamos en LOST, actualizamos el estado según corresponda
    else
    {
        // Si leemos algo diferente de 0 0 0, actualizamos el estado normalmente
        if (this->digitalLeftValue == true && this->digitalMiddleValue == true && this->digitalRightValue == false)
        {
            this->status = CENTERED_SLIGHT_RIGHT;
        }
        if (this->digitalLeftValue == false && this->digitalMiddleValue == true && this->digitalRightValue == true)
        {
            this->status = CENTERED_SLIGHT_LEFT;
        }
        if (this->digitalLeftValue == false && this->digitalMiddleValue == false && this->digitalRightValue == true)
        {
            this->status = RIGHT;
        }
        if (this->digitalLeftValue == true && this->digitalMiddleValue == false && this->digitalRightValue == false)
        {
            this->status = LEFT;
        }
        if ((this->digitalLeftValue == true && this->digitalMiddleValue == true && this->digitalRightValue == true) ||
            (this->digitalLeftValue == false && this->digitalMiddleValue == true && this->digitalRightValue == false))
        {
            this->status = CENTERED;
        }
    }
}

// 4. Funciones auxiliares
String statusToString(LINE_TRACKING_STATUS status)
{
    switch (status)
    {
    case CENTERED:
        return "CENTERED";
    case CENTERED_SLIGHT_LEFT:
        return "CENTERED_SLIGHT_LEFT";
    case CENTERED_SLIGHT_RIGHT:
        return "CENTERED_SLIGHT_RIGHT";
    case LEFT:
        return "LEFT";
    case RIGHT:
        return "RIGHT";
    case LOST_PREV_LEFT:
        return "LOST_PREV_LEFT";
    case LOST_PREV_RIGHT:
        return "LOST_PREV_RIGHT";
    case LOST:
        return "LOST";
    default:
        return "UNKNOWN";
    }
}
